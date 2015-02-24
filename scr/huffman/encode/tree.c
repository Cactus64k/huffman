#include "../huffman.h"

node* create_tree(node_list* list_itm, node* nodes_buff)
{
	assert(list_itm != NULL);
	assert(nodes_buff != NULL);

	node_list list_buff[256];
	size_t index			= 0;
	node_list* search_itm	= list_itm;
	node* new_node			= NULL;

	while(list_itm->next != NULL)
	{
		assert(index < 255);

		new_node	 			= nodes_buff + index;		// новый элемент делева
		node_list* new_itm		= list_buff + index;		// новый элемент списка
		new_itm->nd				= new_node;
		new_itm->next			= NULL;

		node* left				= list_itm->nd;
		node* right				= list_itm->next->nd;

		new_node->left			= left;
		new_node->right			= right;
		new_node->prob			= left->prob + right->prob;


		while(1)
		{
			if(search_itm->next == NULL)	// если конец списка
			{
				search_itm->next = new_itm;
				search_itm = new_itm;

				break;
			}
			else if(new_node->prob < search_itm->next->nd->prob)	// если следующий элемент больше нового
			{
				new_itm->next		= search_itm->next;
				search_itm->next	= new_itm;

				search_itm = new_itm;

				break;
			}

			search_itm = search_itm->next;
		}



		index++;
		list_itm = list_itm->next->next;
	}

	return new_node;
}



