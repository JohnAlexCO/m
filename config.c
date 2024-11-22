#define config_maxsize 1024 * 4
#define config_reset file_write(root_config, root_default_config, sizeof(root_default_config))

list_t *config_load(char *filename){
    /* Initialize an empty configuration */
    list_t *self = list_new(module_alloc_default);

    /* export the default config if none exists */
    char config[config_maxsize];
    int read = file_read(root_config, config, sizeof(config));
    if (read == false) { config_reset; }

    /* Load and begin processing the configuration file */
    int len = file_read(root_config, config, sizeof(config));
    int cursor; int last = 0; int line = 0; int item = 0;
    char *working_name; char *working_bin; char *working_flag; char *working_ext;
    for(cursor=0; cursor < len; cursor++) {
        if ( config[cursor] == '\n' ) {
            if      (working_name == NULL)   { printf(config_missing_name, line); exit(1); }
            else if (working_bin == NULL)    { printf(config_missing_binary, line); exit(1); }
            else if (working_flag == NULL)   { printf(config_missing_flag, line); exit(1); }
            else if (cursor == last)    { printf(config_missing_ext, line); exit(1); }
            working_ext = string_substr(config, last, cursor);
            module_mount(self, working_name, working_bin, working_flag, working_ext);
            working_name = NULL; working_bin = NULL; working_flag = NULL; working_ext = NULL;
            line++; item = 0; last = cursor+1;
        }
        else if ( config[cursor] == ' ' ) {
            char *snippet = string_substr(config, last, cursor);
            switch(item) {
                default: printf(config_unexpected, line); exit(1); break;
                case 0: working_name = snippet; break;
                case 1: working_bin = snippet; break;
                case 2: working_flag = snippet; break;
                /* case 3: working_ext = snippet; break; !! DONE DURING NEWLINE*/
            }   item++; last = cursor+1;
        }
    }
    /* before returning, make sure we don't have a partial entry */
    if (item == 0) { return self; }
    if (item == 1) { printf(config_missing_binary, line); exit(1); }
    if (item == 2) { printf(config_missing_flag); exit(1); }
    if (item == 3) {
        if (cursor == last) {printf(config_missing_flag, line); exit(1); }
        working_ext = string_substr(config, last, cursor);
        module_mount(self, working_name, working_bin, working_flag, working_ext);
        return self;
    }
    printf(config_unexpected, line); exit(1);
}

void config_save(list_t *modules, char *filename) {
    char *config = malloc(sizeof(char)*config_maxsize);
    char *line = malloc(sizeof(char)*512);
    int i; for(i=0;i<modules->entries;i++) {
        module_t *ref = modules->item[i];
        sprintf(line, "%s %s %s %s\n", ref->name, ref->bin, ref->flag, ref->ext);
        config = string_concat(config, line);
    }
    file_write(filename, config, config_maxsize);
    free(config);
    free(line);
}

void config_edit() {
    #ifdef _WIN_32
        system("notepad.exe " root_config);
    #elif defined(__APPLE__)
        system("open -a TextEdit ", root_config);
    #else
        system("nano " root_config);
    #endif
    exit(0);
}