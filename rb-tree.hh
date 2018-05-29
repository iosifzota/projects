#ifndef __rb_tree_hh
#define __rb_tree_hh


	template <typename T, typename T_Node, typename Less>
	void basic_btree<T, T_Node, Less>::print_levels(std::ostream& out) const
	{
		std::queue< pair<shared<T_Node>, unsigned> > fringe;
		shared<T_Node> current_node, temp;
		unsigned prev_level, current_level;

		req(root != nullptr);
		if (root == NIL) {
			return;
		}

		prev_level = 0;
		fringe.push({ root, 0 });

		while (!fringe.empty()) {
			current_node = fringe.front().key;
			current_level = fringe.front().val;

			req(current_node != nullptr, "[Debug]");

			if (current_node != NIL) {
				fringe.push({ current_node->left, current_level + 1 });
				fringe.push({ current_node->right, current_level + 1 });
			}

			if (prev_level != current_level) {
				prev_level = current_level;
				out << '\n';
				 
				/* Left margin. */
				for (unsigned tabs = 0; tabs < current_level % 50; ++tabs) {
					out << ' ';
				}
 			}

			if (current_node != NIL)
				out << current_node->data << ' ';
			else
				out << "  ";

			/* Right padding. */
			for (unsigned tabs = 0; tabs < current_level % 50; ++tabs) {
				out << ' ';
			}

			fringe.pop();
		}
	}

#endif