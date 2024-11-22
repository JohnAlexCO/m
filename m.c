#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <sys/stat.h>
#include "locale/locale.h"
#include "shared/list.c"
#include "shared/string.c"
#include "shared/file.c"
#include "modules.c"
#include "config.c"

void help(){ printf("%s\n%s\n", root_version_info, help_text); }
void version() { printf("%s\n", root_version_info); }

int main(int argc, char ** argv) {

    /* Static options: no need to load the config yet */
    if ( argc == 1 ) { help(); exit(1); }
    if ( argc == 2 ) {
        if ( string_compare("help", argv[1], 8))    { help(); exit(0); }
        if ( string_compare("-h", argv[1], 8))      { help(); exit(0); }
        if ( string_compare("--help", argv[1], 8))  { help(); exit(0); }
        if ( string_compare("version", argv[1], 8)) { version(); exit(0); }
        if ( string_compare("-v", argv[1], 8))      { version(); exit(0); }
        if ( string_compare("--version", argv[1], 12)) { version(); exit(0); }
        if ( string_compare("config", argv[1], 12)) { config_edit(); }
        if ( string_compare("reset", argv[1], 8)){ config_reset; exit(0); }
        if ( string_compare("init", argv[1], 8)) { config_reset; system("mkdir modules"); exit(0); }
        if ( string_compare("license", argv[1], 8)) { printf("%s\n", license_text); exit(0); }
    }

    /* Load in modules and process remaining options before compiling */
    list_t *modules = config_load(root_config);
    if ( argc == 2 ) {
        if ( string_compare("info", argv[1], 12))  { version(); modules_versions(modules); exit(0); }
        if ( string_compare("licenses", argv[1], 12)) { printf("m: GNU GPLv3\n"); modules_licenses(modules); exit(0); }
        if ( string_compare("list", argv[1], 12))  { modules_list(modules); exit(0); }
        if ( string_compare("contributors", argv[1], 13) ) { printf("%s\n", root_contributors); modules_contributors(modules); exit(0); }
    }

    if ( argc == 3 ) {
        if( string_compare("unmount", argv[1], 8)) { module_unmount(modules, argv[2]); config_save(modules, root_config); exit(0); }
        if( string_compare("remove", argv[1], 8)) { module_remove(modules, argv[2]); config_save(modules, root_config); exit(0); }
    }

    if ( argc == 6 ) {
        if ( string_compare("install", argv[1], 8)) {
            char *name = argv[2];
            char *flag = argv[3];
            char *ext = argv[4];
            char *url = argv[5];
            module_install(modules, name, flag, ext, url);
            config_save(modules, root_config); exit(0);
        }
        if ( string_compare("mount", argv[1], 8)) {
            char *name = argv[2];
            char *flag = argv[3];
            char *ext = argv[4];
            char *bin = argv[5];
            module_mount(modules, name, bin, flag, ext);
            config_save(modules, root_config); exit(0);
        }
    }

    /* TODO:
        install (download, mount, export config)
        mount (mount, export config)
        && if no cli option caught, process arguments as commands to compilers
    */
}