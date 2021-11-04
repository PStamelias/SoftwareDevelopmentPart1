#include "fun.h"
#include <stdio.h>
int main(){
	struct Info_Table* Word=deduplication_method("out.txt");
	entry_list* el=NULL;
	Index* ix=NULL;
	create_entry_list(&el);
	for(int i=0;i<Word->counter;i++){
		for(int j=0;j<Word->nodes->counter;j++){
			const word* k=Word->nodes->names[j];
			entry* en=NULL;
			create_entry(k,&en);
			const entry* w=en;
			add_entry(el,w);
		}
	}
	const entry_list* my=el;
	build_entry_index(my,2,&ix);
	struct Node_Index* r=(*ix).Nodeptr;
	for(int i=0;i<r->child_counter;i++){
		printf("%d\n",r->Root_Node[i].Id_root);
		printf("%s\n",r->Root_Node[i].childptr->the_word);
		if(r->Root_Node[i].Id_root==2){
			struct Node_Index* re=r->Root_Node[i].childptr;
			for(int j=0;j<re->child_counter;j++){
				printf("--%d\n",re->Root_Node[j].Id_root);
				printf("--%s\n",re->Root_Node[j].childptr->the_word);
			}
		}
	}
	destroy_entry_index(ix);
	destroy_entry_list(el);
	delete_name_info(Word);
	return 0;
}
