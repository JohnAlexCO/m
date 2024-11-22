typedef struct {
    int entries; int size;
    void **item;
} list_t;

void list_print(list_t *self){
    printf("list = { %d entries\n", self->entries);
    int i; for(i=0; i<self->entries; i++) {
        printf("    %d: %x\n", i, self->item[i]);
    }   printf("}\n");
}

list_t *list_new(int size){
    list_t *self = malloc(sizeof(list_t));
    self->entries = 0;
    self->size = size;
    self->item = malloc(sizeof(void*) * self->size);
    return self;
}

void list_add(list_t *self, void *item) {
    if (self->entries == self->size) {
        self->size *=2;
        self->item = realloc(self->item, sizeof(void*) * self->size);
    }   self->item[self->entries] = item;
    self->entries++;
}

void list_remove(list_t *self, int index) {
    int i; for(i=index; i<self->entries; i++) {
        self->item[i] = self->item[i+1];
    }
    if ( self->size < (self->entries/2) ) {
        self->size /=2;
        self->item = realloc(self->item, sizeof(void*) * self->size);
    }   self->entries--;
}

void list_concat(list_t *self, list_t *ref) {
    int i; for(i=0; i<ref->entries; i++) {
        list_add(self, ref->item[i]);
    }
}

bool list_find(list_t *self, void *item) {
    int i; for(i=0; i<self->entries; i++) {
        if (self->item[i] == item) { return true; }
    }   return false;
}