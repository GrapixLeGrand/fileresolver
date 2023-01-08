#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include <memory>
#include <stdexcept>
#include <queue>
#include <unordered_set>

namespace Grapix {

class FileSystemException : public std::runtime_error {
public:
    /// Variadic template constructor to support printf-style arguments
    template <typename... Args> FileSystemException(const char *fmt) 
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
            throw FileSystemException("path cannot be empty !");
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
            throw FileSystemException("path is already full !");
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
            throw FileSystemException("getAbsolute requires a relative path");
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
            throw FileSystemException("failed to find the origin path!");
        }
    }

};



class RecursiveFileResolver : public FileResolver {
private:
    int m_maxDepth;

public:
    RecursiveFileResolver(const std::string& originDirectory, int maxDepth): m_maxDepth(maxDepth) {

        std::string compiledPathStr = getCompiledPath();
        std::filesystem::path compiledPath(compiledPathStr);

        std::queue<std::tuple<std::filesystem::path, bool, int>> exploredPaths;
        std::tuple<std::filesystem::path, bool, int> root = std::make_tuple(compiledPath, true, 0);
        exploredPaths.push(root);

        std::unordered_set<std::string> visitedFolders;

        while (!exploredPaths.empty()) {
            auto v = exploredPaths.front();
            exploredPaths.pop();

            SubPathBuilder currentPathBuilder(std::get<0>(v));

            //check if the current path is the goal
            

            //find all outgoings ways that were not already visited (cd .. included if not root)

            //push all egdes on the queue

        }

    }

};

class FileResolverFactory {
private:
    FileResolverFactory() {}
public:
    static FileResolver makeLinearResolver(const std::string& originDirectory) {
        return LinearFileResolver(originDirectory);
    }
    static FileResolver makeRecursiveResolver(const std::string& originDirectory, int maxDepth = 5) {
        return RecursiveFileResolver(originDirectory, maxDepth);
    }
};


}