/*
    Modules are external binaries, called by the m cli,
    which either return status code 0 for "good",
    or any other value to indicate an error.
    The arguments they accept should be `module {input.file} {output.file}`
    and the error should be written to the output file still in the structured form
        uint32_t line,
        uint32_t column,
        {stream of error trace}
*/
#define module_max_namesize 128
#define module_alloc_default 64
typedef struct {
    char *name;
    char *bin;
    char *flag;
    char *ext;
} module_t;

char *module_fromName(list_t *modules, char *name) {
    int i=0; for(i=0; i<modules->entries;i++) {
        module_t *ref = modules->item[i];
        if (string_compare(ref->name, name, module_max_namesize)) { return ref->bin; }
    }   return NULL;
}

char *module_fromFilename(list_t *modules, char *filename) {
    /* Figure out the file extension */
    char *ext = NULL; int i;
    int len = strnlen(filename, module_max_namesize);
    for(i=0;i<len;i++) { if (filename[i]=='.'){ ext = (char*)(filename + i); }} if (ext == NULL) { return ext; }
    /* Then attempt to match it */
    for(i=0;i<modules->entries;i++) {
        module_t *ref = modules->item[i];
        if (string_compare(ref->ext, ext, module_max_namesize)) { return ref->bin; }
    }   return NULL;
}

void module_mount(list_t *modules, char *name, char *binary, char *flag, char *extension){
    module_t *self = malloc(sizeof(module_t));
    self->name = name; self->bin = binary; self->flag = flag; self->ext = extension;
    list_add(modules, self);
}

void module_unmount(list_t *modules, char *name) {
    int i; for(i=0;i<modules->entries;i++) {
        module_t *ref = modules->item[i];
        if (string_compare(ref->name, name, module_max_namesize)==true) {
            printf("module_unmount: removing %s (%d)\n", ref->bin, i);
            list_remove(modules, i); return;
        }
    }   printf("module_unmount: no module \"%s\" was found\n", name);
}

void module_call(list_t *modules, char *name, char *filename, char *output) {
    /* Check to make sure the module actually exists */
    char command_path[1024]; char module_path[1024];
    char *module_binary = module_fromName(modules, name);
    if (module_binary == NULL) { printf(root_missing_module, name); exit(1); }
    sprintf(module_path, "./modules/%s", module_binary); file_route(module_path);
    snprintf(command_path, sizeof(command_path), "%s %s %s", module_path, filename, output);
    if (file_exists(module_path) == false) { printf(root_missing_binary, name); exit(1); }
    uint8_t code = system(command_path) % 255;
    if (code != 0) {
        uint32_t line; uint32_t column;
        char fileBuffer[1024 * 4]; file_read(output, fileBuffer, sizeof(fileBuffer));
        printf(root_error, name, code, fileBuffer);
        file_delete(output);
        exit(code);
    }
}

void modules_list(list_t *modules) {
    printf("%d modules in the local config:\n", modules->entries);
    int i=0; for(i=0; i < modules->entries;i++) {
        module_t *ref = modules->item[i];
        printf("    %s [%s] (*%s) => %s\n", ref->name, ref->flag, ref->ext, ref->bin );
    }   exit(0);
}

int module_download(char *filename, char *url){
    char command_path[1024]; char module_path[1024];
    sprintf(module_path, "./modules/%s", filename); file_route(module_path);
    #ifdef _WIN_32
        snprintf(command_path, sizeof(command_path), "Invoke-WebRequest -Uri \"%s\" -OutFile \"%s\"", url, filename);
    #else
        snprintf(command_path, sizeof(command_path), "curl -o %s %s", filename, url);
    #endif
    return system(command_path);
}

void module_install(list_t *modules, char *name, char *flag, char *ext, char *url){
    // get last slash in name
    int s=0; int i; for(i=0;i<strlen(url);i++) { if (url[i]=='/') { s=i; } }
    // set filename to string after last slash
    char *filename = (char*)( url + s + 1);
    // then download and mount
    char module_path[1024]; sprintf(module_path, "./modules/%s", filename); file_route(module_path);
    int code = module_download(module_path, url);
    if ( code !=0 ) { printf("module_install: error downloading file, given code %d\n", code); exit(1); }
    module_mount(modules, name, filename, flag, ext);
}

void module_remove(list_t *modules, char *name) {
    char file_path[1024];
    char *bin = module_fromName(modules, name);
    if (bin == NULL) { printf("module_remove: no binary associated with \"%s\"\n", name); exit(1); }
    printf("module_remove: deleting /modules/%s\n", bin);
    sprintf(file_path, "./modules/%s", bin); module_unmount(modules, name);
    file_delete(file_path);
}

void modules_contributors(list_t *modules){
    char command_path[1024]; char module_path[1024];
    int i=0; for(i=0; i < modules->entries;i++) {
        module_t *ref = modules->item[i];
        sprintf(module_path, "./modules/%s", ref->bin);
        file_route(module_path);
        snprintf(command_path, sizeof(command_path), "%s --contributors", module_path);
        system(command_path);
    }
}

void modules_versions(list_t *modules){
    char command_path[1024]; char module_path[1024];
    int i=0; for(i=0; i < modules->entries;i++) {
        module_t *ref = modules->item[i];
        sprintf(module_path, "./modules/%s", ref->bin);
        file_route(module_path);
        snprintf(command_path, sizeof(command_path), "%s --version", module_path);
        system(command_path);
    }
}

void modules_licenses(list_t *modules){
    char command_path[1024]; char module_path[1024];
    int i=0; for(i=0; i < modules->entries;i++) {
        module_t *ref = modules->item[i];
        sprintf(module_path, "./modules/%s", ref->bin);
        file_route(module_path);
        snprintf(command_path, sizeof(command_path), "%s --license", module_path);
        system(command_path);
    }
}