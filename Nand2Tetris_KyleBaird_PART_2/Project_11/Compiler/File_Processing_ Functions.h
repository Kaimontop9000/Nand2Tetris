/*FILE PROCESSING FUNCTIONS

void make_output_filename(const char *input_filename, char *output_filename, size_t size)

file processing handled in main. No functions yet, should probably make functions for 
file processing.*/


void make_output_filename(const char *input_filename, char *output_filename, size_t size) {
    const char *dot = strrchr(input_filename, '.');
    size_t base_len = dot ? (size_t)(dot - input_filename) : strlen(input_filename);

    // Make sure there's room for base name + "T.xml" + null terminator
    if (base_len + 4 >= size) base_len = size - 5;

    strncpy(output_filename, input_filename, base_len);
    output_filename[base_len] = '\0';
    strncat(output_filename, ".xml", size - base_len - 1);
}


/*Symbol Tables
name:(name of identifier)
type:(int, char, boolean or class type)
kind:(field,static,arg or var(local))
#:(index of kind)
category:(class or subroutine)
usage:(declared or used)
proposed XML output
</identifier> name:%s, type:%s, kind:%s, #:%d, category:%s, usage:%s </identifier>*/