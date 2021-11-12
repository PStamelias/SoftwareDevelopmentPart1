#include "acutest.h"
#include "fun.h"
void test_create_entry(void)
{
   const word* a="name"; 
   entry* start=NULL;
   enum error_code co;
   co=create_entry(a,&start);
   TEST_CHECK(start != NULL);
   TEST_CHECK(!strcmp(a,start->my_word));
   TEST_CHECK(co != EC_FAIL);
}
void test_destroy_entry(void){
   const word* a="name"; 
   entry* start=NULL;
   create_entry(a,&start);
   enum error_code co=destroy_entry(start);
   TEST_CHECK(co != EC_FAIL);
}
void test_create_entry_list(void){
   entry_list* node=NULL;
   enum error_code co=create_entry_list(&node);
   TEST_CHECK(co != EC_FAIL);
   TEST_CHECK(node != NULL);
}
void test_add_entry(void){
   const word* a="name"; 
   entry* start=NULL;
   create_entry(a,&start);
   entry_list* node=NULL;
   create_entry_list(&node);
   enum error_code co=add_entry(node,start);
   entry* s=get_first(node);
   TEST_CHECK(!strcmp(s->my_word,a));
   TEST_CHECK(co != EC_FAIL);
}
void test_get_number_entries(void){
   const word* a="name"; 
   entry* start=NULL;
   create_entry(a,&start);
   entry_list* node=NULL;
   create_entry_list(&node);
   add_entry(node,start);
   int coun=get_number_entries(node);
   TEST_CHECK(coun==1);
}
void test_get_first(void){
   const word* a="name"; 
   entry* start=NULL;
   create_entry(a,&start);
   entry_list* node=NULL;
   create_entry_list(&node);
   add_entry(node,start);
   entry* k=get_first(node);
   TEST_CHECK(k!=NULL);
   TEST_CHECK(!strcmp(k->my_word,a));
}
void test_get_next(void){
   const word* a="name";
   const word* b="next_name";
   entry* start=NULL;
   entry* new_start=NULL;
   create_entry(a,&start);
   create_entry(b,&new_start);
   entry_list* node=NULL;
   create_entry_list(&node);
   add_entry(node,start);
   add_entry(node,new_start);
   entry* x=get_next(node,start);
   TEST_CHECK(x!=NULL);
   TEST_CHECK(!strcmp(x->my_word,new_start->my_word));
}
void test_destroy_entry_list(void){
   const word* a="name";
   const word* b="next_name";
   entry* start=NULL;
   entry* new_start=NULL;
   create_entry(a,&start);
   create_entry(b,&new_start);
   entry_list* node=NULL;
   create_entry_list(&node);
   add_entry(node,start);
   add_entry(node,new_start);
   enum error_code co=destroy_entry_list(node);
   TEST_CHECK(co != EC_FAIL);
}
void test_build_entry_index(void){
   struct Name_Info* Word=deduplication_method("out.txt");
   entry_list* el=NULL;
   Index* ix=NULL;
   create_entry_list(&el);
   for(int i=0;i<Word->counter;i++){
      struct Name* d=Word->ptr[i];
      while(1){
         const word* k=d->the_name;
         entry* en=NULL;
         create_entry(k,&en);
         const entry* w=en;
         add_entry(el,w);
         d=d->next;
         if(d==NULL)
            break;
      }
   }
   const entry_list* my=el;
   enum error_code a=build_entry_index(my,2,&ix);
   TEST_CHECK(a != EC_FAIL);
   TEST_CHECK(ix != NULL);
}
void test_lookup_entry_index(void){
   struct Name_Info* Word=deduplication_method("out.txt");
   entry_list* el=NULL;
   Index* ix=NULL;
   create_entry_list(&el);
   for(int i=0;i<Word->counter;i++){
      struct Name* d=Word->ptr[i];
      while(1){
         const word* k=d->the_name;
         entry* en=NULL;
         create_entry(k,&en);
         const entry* w=en;
         add_entry(el,w);
         d=d->next;
         if(d==NULL)
            break;
      }
   }
   const entry_list* my=el;
   build_entry_index(my,2,&ix);
   entry_list* oth=NULL;
   //enum error_code a=lookup_entry_index();
   //tsekare an i lista entry_list einai diafori toy NULL
   //TEST_CHECK(el != NULL);
   //TEST_CHECK(a != EC_FAIL);
}
void test_destroy_entry_index(void){
   struct Name_Info* Word=deduplication_method("out.txt");
   entry_list* el=NULL;
   Index* ix=NULL;
   create_entry_list(&el);
   for(int i=0;i<Word->counter;i++){
      struct Name* d=Word->ptr[i];
      while(1){
         const word* k=d->the_name;
         entry* en=NULL;
         create_entry(k,&en);
         const entry* w=en;
         add_entry(el,w);
         d=d->next;
         if(d==NULL)
            break;
      }
   }
   const entry_list* my=el;
   build_entry_index(my,2,&ix);
   enum error_code con=destroy_entry_index(ix);
   TEST_CHECK(con != EC_FAIL);
}
void test_edit_distance(void){
   int distance=edit_distance("belt","hell",0);
   TEST_CHECK(distance == 2);
}
void test_hamming_distance(void){
   int distance=hamming_distance("belt","hell");
   TEST_CHECK(distance == 2);
}
void test_deduplication_method(void){
   struct Name_Info* Word=deduplication_method("out.txt");
   TEST_CHECK(Word != NULL);
}
void test_delete_name_info(void){
   struct Name_Info* Word=deduplication_method("out.txt");
   enum error_code co=delete_name_info(Word);
   TEST_CHECK(co != EC_FAIL);
}
TEST_LIST = {
   {"create_entry",test_create_entry},
   {"destroy_entry",test_destroy_entry},
   {"create_entry_list",test_create_entry_list},
   {"get_number_entries",test_get_number_entries},
   {"add_entry",test_add_entry},
   {"get_first",test_get_first},
   {"get_next",test_get_next},
   {"destroy_entry_list",test_destroy_entry_list},
   {"build_entry_index",test_build_entry_index},
   {"lookup_entry_index",test_lookup_entry_index},
   {"destroy_entry_index",test_destroy_entry_index},
   {"edit_distance",test_edit_distance},
   {"hamming_distance",test_hamming_distance},
   {"deduplication_method",test_deduplication_method},
   {"delete_name_info",test_delete_name_info},
   /*{"destroy_index_nodes",test_destroy_index_nodes},-- den ti tsekarame dioti den ekane free ti mnimi */
   { NULL,NULL }
};