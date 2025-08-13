#ifndef FILE_PROCESSING_H 
#define FILE_PROCESSING_H  
/*Created August 11th - Last edited August 11th*/
/*FILE PROCESSING FUNCTIONS

void make_output_filename(const char *input_filename, char *output_filename, size_t size)

file processing handled in main. No functions yet, should probably make functions for 
file processing.*/

void make_output_filename(const char *input_filename, char *output_filename, size_t size);

/*Symbol Tables
name:(name of identifier)
type:(int, char, boolean or class type)
kind:(field,static,arg or var(local))
#:(index of kind)
category:(class or subroutine)
usage:(declared or used)
proposed XML output
</identifier> name:%s, type:%s, kind:%s, #:%d, category:%s, usage:%s </identifier>*/

#endif // End of include guard