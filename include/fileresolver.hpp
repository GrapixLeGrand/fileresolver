#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include <memory>
#include <stdexcept>
#include <queue>
#include <unordered_set>
#include <sstream>

namespace Grapix {

class FileResolverException : public std::runtime_error {
public:
    /// Variadic template constructor to support printf-style arguments
    template <typename... Args> FileResolverException(const char *fmt) 
     : std::runtime_error(fmt) { }
};

class SubPathBuilder {
private:
    std::filesystem::path m_path;
    std::vector<std::string> m_pathElements;
    size_t m_currentHead = 0;
public:
    
    /**
     * @brief allows to create subpath from a full path
     * 
     * @param path 
     */
    SubPathBuilder(std::filesystem::path path) {
        m_path = path;
        m_pathElements = std::vector<std::string>(m_path.begin(), m_path.end());
        //m_pathLen = m_pathElements.size();
        m_currentHead = m_pathElements.size() - 1;
    }

    void previous() {
        if (m_currentHead == 0) {
            throw FileResolverException("path cannot be empty !");
        }
        std::filesystem::path nextPath;
        m_currentHead--;
        for (int i = 0; i <= m_currentHead; i++) {
            nextPath /= m_pathElements[i];
        }
        m_path = nextPath;
    }

    void next() {
        if (m_currentHead == m_pathElements.size() - 1) {
            throw FileResolverException("path is already full !");
        }
        std::filesystem::path nextPath;
        m_currentHead++;
        for (int i = 0; i <= m_currentHead; i++) {
            nextPath /= m_pathElements[i];
        }
        m_path = nextPath;
    }

    size_t getInitialLength() {
        return m_pathElements.size();
    }

    const std::filesystem::path getPath() {
        return m_path;
    }

    const std::string getCurrentHead() {
        return m_pathElements[m_currentHead];
    }
};





class FileResolver {
protected:
    std::filesystem::path m_originPath;
    FileResolver() {}
public:

    /**
     * @brief returns the absolute path of the file given a relative path from the origin
     * 
     * @param relativePath 
     * @return std::string 
     */
    virtual std::string getAbsolute(const std::string& relativePathStr) const {
        std::filesystem::path relativePath (relativePathStr);
        if (relativePath.is_absolute()) {
            throw FileResolverException("getAbsolute requires a relative path");
        }
        return m_originPath / relativePath;
    }

    /**
     * @brief Returns the path from which the filesystem is compiled
     * 
     * @return const std::string 
     */
    virtual std::string getCompiledPath() const {
        return std::filesystem::current_path();
    }

    virtual std::string getOriginPath() const {
        return m_originPath;
    }

    virtual std::string toString() const { return "FileResolver"; };

};

class LinearFileResolver : public FileResolver {
public:

    LinearFileResolver(const std::string& originDirectory) {

        std::string compiledPathStr = getCompiledPath();
        std::filesystem::path compiledPath(compiledPathStr);

        SubPathBuilder pathBuilder (compiledPath);

        bool found = false;
        for (int i = pathBuilder.getInitialLength(); i >= 0; i--) {
            auto currentPath = pathBuilder.getPath();
            if (std::filesystem::is_directory(currentPath) && pathBuilder.getCurrentHead() == originDirectory) {
                found = true;
                m_originPath = currentPath;
                break;
            }
            pathBuilder.previous();
        }

        if (!found) {
            throw FileResolverException("failed to find the origin path!");
        }
    }

    virtual std::string toString() const override {
        std::ostringstream ss;
        ss << "LinearFileResolver with path: " << m_originPath;
        return ss.str();
    }

};


/**
 * @brief This resolver will perform a breath-first-search over the files of the project. It will discover until maxDepth
 * and will throw a FileResolverException if the origin folder wasn't found.
 */
class RecursiveFileResolver : public FileResolver {
private:
    int m_maxDepth;
    int m_numIterations = 0;
    int m_numVisitedFolders = 0;

    /**
     * @brief return all the paths to folders from the currentPath. In addition an entry to the tail path (cd ..)
     * is also added
     * 
     * @param currentPath 
     * @return std::filesystem::path 
     */
    static std::vector<std::filesystem::path> lsDirectories(const std::filesystem::path& currentPath) {

        if (!std::filesystem::is_directory(currentPath)) {
            throw FileResolverException("cannot list folders for a non-directory path");
        }

        std::vector<std::filesystem::path> result;
        for (auto p : std::filesystem::directory_iterator(currentPath)) {
            if (std::filesystem::is_directory(p) && !std::filesystem::is_empty(p)) {
                result.push_back(p);
            }
        }

        result.push_back(getTailPath(currentPath));

        return result;
    }

    /**
     * @brief returns the same std::filesystem::path without the head
     * 
     * @param currentPath 
     * @return std::filesystem::path 
     */
    static std::filesystem::path getTailPath(const std::filesystem::path& currentPath) {
        
        if (currentPath.empty()) {
            return std::filesystem::path{};
        }
        
        auto end = currentPath.end();
        end--;
        std::filesystem::path result;

        for (auto start = currentPath.begin(); start != end; start++) {
            result /= *start;
        }

        return result;
    }

    /**
     * @brief returns the head of a std::filesystem::path
     * 
     * @param currentPath 
     * @return std::string 
     */
    static std::string getHead(const std::filesystem::path& currentPath) {
        if (currentPath.empty()) {
            return "";
        }
        auto end = currentPath.end();
        end--;
        return (*end).string();
    }

public:
    RecursiveFileResolver(const std::string& originDirectory, int maxDepth): m_maxDepth(maxDepth) {

        std::string compiledPathStr = getCompiledPath();
        std::filesystem::path compiledPath(compiledPathStr);

        std::queue<std::pair<std::filesystem::path, int>> exploredPaths;
        std::pair<std::filesystem::path, int> root = std::make_pair(compiledPath, 0);
        exploredPaths.push(root);

        std::unordered_set<std::string> visitedFolders;
        visitedFolders.insert(root.first.string());

        bool found = false;
        while (!exploredPaths.empty()) {
            auto currentPathTuple = exploredPaths.front();
            exploredPaths.pop();

            //check if the current path is the goal
            if (getHead(currentPathTuple.first) == originDirectory) {
                found = true;
                m_originPath = currentPathTuple.first;
                break;
            }
            
            //reached maximum lookup depth
            if (currentPathTuple.second >= maxDepth) {
                continue;
            }

            //find all outgoings ways that were not already visited (cd .. included if not root)
            std::vector<std::filesystem::path> nextPaths = lsDirectories(currentPathTuple.first);

            //push all egdes on the queue
            for (auto& nextPath : nextPaths) {
                if (visitedFolders.find(nextPath.string()) == visitedFolders.end()) {
                    visitedFolders.insert(nextPath.string());
                    exploredPaths.push(std::make_pair(nextPath, currentPathTuple.second + 1));
                }
            }

            m_numIterations++;
        }
        
        m_numVisitedFolders = visitedFolders.size();

        if (!found) {
            throw FileResolverException("failed to find the origin path!");
        }

    }

    virtual std::string toString() const override {
        std::ostringstream ss;
        ss << "RecursiveFileResolver with path: " << m_originPath;
        ss << "\n" << "terminated in " << m_numIterations << " iterations and visited " << m_numVisitedFolders << " folders";
        return ss.str();
    }

};

class FileResolverFactory {
private:
    FileResolverFactory() {}
public:
    
    static FileResolver makeLinearResolver(const std::string& originDirectory) {
        return LinearFileResolver(originDirectory);
    }

    static FileResolver* makeLinearResolverPtr(const std::string& originDirectory) {
        return new LinearFileResolver(originDirectory);
    }
    
    static FileResolver makeRecursiveResolver(const std::string& originDirectory, int maxDepth = 5) {
        return RecursiveFileResolver(originDirectory, maxDepth);
    }

    static FileResolver* makeRecursiveResolverPtr(const std::string& originDirectory, int maxDepth = 5) {
        return new RecursiveFileResolver(originDirectory, maxDepth);
    }

};


}