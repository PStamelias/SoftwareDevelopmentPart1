typedef enum MatchType{
  MT_EXACT_MATCH,
  MT_HAMMING_DIST,
  MT_EDIT_DIST
}MatchType;			
typedef enum error_code{
  EC_SUCCESS,
  EC_NO_AVAIL_RES,
  EC_FAIL
}error_code;
typedef char word;
typedef struct payload_node{
	word* my_word;
	struct payload_node* next;
}payload_node;
typedef struct entry {
  word* my_word;
  payload_node* payload;
  struct entry* next;
}entry;
typedef struct entry_list{
  entry* first_node;
  entry* current_node;
  unsigned int counter;
}entry_list;
struct Info_Table{
  struct Name_info* nodes;
  int counter;
};
struct Name_info{
  char** names;
  int counter;
};
struct root{
  int Id_root;
  struct Node_Index* childptr;
};
struct Node_Index{
  word* the_word;
  int child_counter;
  struct root* Root_Node;
};
typedef struct Index{
  struct Node_Index* Nodeptr;
}Index;
enum error_code create_entry();
enum error_code destroy_entry(entry* e);
enum error_code create_entry_list(entry_list** el);
unsigned int get_number_entries(const entry_list* el);
enum error_code add_entry(entry_list* el,const entry* e);
entry* get_first(const entry_list* el);
entry* get_next(const entry_list* el,const entry* e);
enum error_code destroy_entry_list(entry_list* el);
enum error_code build_entry_index(const entry_list* el,MatchType type,Index** ix);
enum error_code lookup_entry_index(const word* w,Index* ix,int threshold,entry_list** result);
enum error_code destroy_entry_index(Index* ix);
int edit_distance(char*, char*, int);    //complexity: O(n(m-n)), m is the length of the largest word, n is the length of the shortest word
int hamming_distance(char*, char*); //complexity: O(n), n is the length of the shortest word
void delete_name_info(struct Info_Table* node);
struct Info_Table* deduplication_method(char* filename);
void Destroy_Index_Node(struct Node_Index* node);