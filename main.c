#include "fun.h"
#include <stdio.h>
int main(int argc, char **argv){
	struct Name_Info* start_list1=deduplication_method(argv[1]);
	struct Name_Info* start_list2=start_list1;
	entry_list* el=NULL;
	Index* ix=NULL;
	create_entry_list(&el);
	while(1){
		struct Name* e=start_list1->ptr;
		while(1){
			const word* k=e->the_name;
			printf("%s\n",k);
			entry* en=NULL;
			create_entry(k,&en);
        	const entry* w=en;
        	add_entry(el,w);
			e=e->next;
			if(e==NULL)
				break;
		}
		start_list1=start_list1->next;
		if(start_list1==NULL)
			break;
	}
	const entry_list* my=el;
	build_entry_index(my,2,&ix);
	//printf("%s\n",ix->root->firstChild->next->firstChild->next->wd);
	destroy_entry_index(ix);
	destroy_entry_list(el);
	delete_name_info(start_list2);
	return 0;
}
