/* REQUIRES: <math.h> */
int string_compare_maxmatch = false;
bool string_compare(char *a, char *b, int max){
    int i=0;
    while(i < max){
        /* checking null terminators */
        if (a[i] == 0 && b[i] == 0) { return true; }
        else if (a[i] == 0 || b[i] == 0) { return false; }
        /* check matching literals */
        if (a[i] != b[i]) { return false; }
    i++; }
        /* if we've made it to the end of max, what we return depends on a flag */
        return string_compare_maxmatch;
}

int string_parseInt(char *a, int max) {
    int i=0; int v=0;
    while(i < max) {
        if (i==0 && a[i]=='-') { v = -0; }
        else if (a[i] == 0) { break; }
        else if (a[i] <= '9' && a[i] >= '0')
            { v*=10; v+=(a[i]-'0'); }
        else { break; }
    i++; }
        return v;
}

int string_parseHex(char *a, int max) {
    int i=0; int v=0;
    while(i < max) {
        if (i==0 && a[i]=='0' && a[i+1]=='x') { i = 1; }
        else if (a[i] == 0) { break; }
        else if (a[i] <= '9' && a[i] >= '0')
            { v*=16; v+=(a[i]-'0'); }
        else if (a[i] <= 'f' && a[i] >= 'a')
            { v*=16; v+=(a[i]-'f' + 15); }
        else { break; }
    i++; }
        return v;
}

float string_parseFloat(char *a, int max) {
    int i=0; int v=0; float r; int point=-1;
    while(i < max) {
        if (a[i] == 0) { break; }
        if (a[i] <= '9' && a[i] >= '0') {
            v*=10; v+=(a[i]-'0');
        }
        else if (point == -1 && a[i]=='.') { point = i; }
        else { break; }
    i++; }
        r = (float)( v / pow((float)10, (float)point+1) );
        return r;
}

char *string_substr(char *a, int start, int end) {
    int length = end - start;
    char *result = malloc(length +1);
    result[length] = 0;
    int i; for(i=0; i<length; i++) { result[i] = a[start+i]; if(a[i]==0) { break; } }
    return result;
}

char *string_concat(char *self, char *add) {
    int len = strlen(self) + strlen(add);
    char *result = malloc(sizeof(char)*len+1);
    sprintf(result, "%s%s\0", self, add);
    return result;
}