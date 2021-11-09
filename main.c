#include "fun.h"
#include <stdio.h>
int main(int argc, char **argv){
	struct Name_Info* Word=deduplication_method(argv[1]);
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
	printf("%s\n",ix->root->firstChild->next->firstChild->next->wd);
	destroy_entry_index(ix);
	destroy_entry_list(el);
	delete_name_info(Word);
	return 0;
}
