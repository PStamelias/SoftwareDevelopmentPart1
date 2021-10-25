typedef enum MatchType{
  MT_EXACT_MATCH,
  MT_HAMMING_DIST,
  MT_EDIT_DIST
}
MatchType;			
typedef enum error_code{
  EC_SUCCESS,
  EC_NO_AVAIL_RES,
  EC_FAIL
}
error_code;
typedef char word;
typedef struct payload_node{
	word* my_word;
	struct payload_node* next;
}payload_node;
typedef struct entry {
  word* my_word;
  payload_node* payload;
}entry;
typedef struct entry_list{
  entry* first_node;
  entry* current_node;
}entry_list;
enum error_code create_entry();
enum error_code destroy_entry(entry* e);
enum error_code create_entry_list(entry_list* el);
unsigned int get_number_entries(const entry_list* el);
enum error_code add_entry(entry_list* el,const entry* e);
entry* get_first(const entry_list* el);
entry* get_next(const entry_list* el);
enum error_code destroy_entry_list(entry_list* el);
/*enum error_code build_entry_index(const entry_list* el,enum match_type,index* ix);
enum error_Code lookup_entry_index(const word* w,index* ix,int threshold,entry_list* result);
enum error_code destroy_entry_index(index* ix);*/