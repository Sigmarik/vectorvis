#include "plugin_loader.h"

#include <dirent.h>
#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "Plug/Plugin.h"
#include "filter_palette.h"
#include "tool_palette.h"

static const size_t MAX_PATH_LENGTH = 1024;

static void load_plugin(const char* path);

static void search_dir(char path[MAX_PATH_LENGTH]);

void load_plugins() {
    static char path[MAX_PATH_LENGTH] = ".";

    search_dir(path);
}

void search_dir(char path[MAX_PATH_LENGTH]) {
    int dir_count = 0;
    struct dirent* dent;
    DIR* src_dir = opendir(path);

    size_t roadblock = strlen(path);

    if (src_dir == NULL) {
        printf("Could not open directory %s\n", path);
        return;
    }

    while ((dent = readdir(src_dir)) != NULL) {
        struct stat st;

        if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
            continue;

        if (fstatat(dirfd(src_dir), dent->d_name, &st, 0) < 0) {
            continue;
        }

        path[roadblock] = '/';
        strcpy(path + roadblock + 1, dent->d_name);

        if (S_ISDIR(st.st_mode)) {
            search_dir(path);
        } else {
            size_t length = strlen(dent->d_name);

            if (length >= 3 && strcmp(dent->d_name + length - 3, ".so") == 0) {
                load_plugin(path);
            }
        }

        path[roadblock] = '\0';
    }

    closedir(src_dir);
}

typedef plug::Plugin* plugin_loader_t();

static void load_plugin(const char* path) {
    printf("Loading plugin %s\n", path);

    void* handle = dlopen(path, RTLD_NOW | RTLD_LOCAL | RTLD_NODELETE);
    if (!handle) {
        printf("Failed to load plugin %s. Reason:\n%s\n", path, dlerror());
        return;
    }

    void* symbol = dlsym(handle, "loadPlugin");
    if (!symbol) {
        printf("Failed to locate plugin loader for plugin %s. Reason:\n%s\n",
               path, dlerror());
        return;
    }

    plugin_loader_t* loader = (plugin_loader_t*)symbol;

    plug::Plugin* plugin = loader();

    plug::Filter* filter = (plug::Filter*)plugin->tryGetInterface(
        (size_t)plug::PluginGuid::Filter);

    if (filter) {
        printf("Loaded filter %s\n", plugin->getPluginData()->getName());
        plugin->addReference();
        FilterPalette::addFilter(*filter);
    }

    plug::Tool* tool =
        (plug::Tool*)plugin->tryGetInterface((size_t)plug::PluginGuid::Tool);

    if (tool) {
        printf("Loaded tool %s\n", plugin->getPluginData()->getName());
        plugin->addReference();
        ToolPalette::addTool(*tool);
    }
}
