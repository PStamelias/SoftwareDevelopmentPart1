typedef enum{
    MT_EXACT_MATCH,
    MT_HAMMING_DIST,
    MT_EDIT_DIST
}
MatchType;			
typedef enum{
    EC_SUCCESS,
    EC_NO_AVAIL_RES,
    EC_FAIL
}
ErrorCode;
typedef char word;
enum error_code create_entry(const word* w,entry* e);
enum error_code destroy_entry(entry* e);
enum error_code create_entry_list(entry_list* el);
unsigned int get_number_entries(const entry_list* el);
enum error_code add_entry(entry_list* el,const entry* e);
entry* get_first(const entry_list* el);
entry* get_next(const entry_list* el);
enum error_code destroy_entry_list(entry_list* el);
enum error_code build_entry_index(const entry_list* el,enum match_type,index* ix);
enum error_Code lookup_entry_index(const word* w,index* ix,int threshold,entry_list* result);
enum error_code destroy_entry_index(index* ix);