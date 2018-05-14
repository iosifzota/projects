#ifndef __rb_tree_hh
#define __rb_tree_hh


	generic (T, Less)
	void rb_tree<T, Less>::print_levels(std::ostream& out) const
	{
		shared_rb_node<T> current_node, temp;
		unsigned prev_level, current_level;
		std::queue< pair<shared_rb_node<T>, unsigned> > fringe;

		req(root != nullptr);
		if (root == NIL) {
			return;
		}

		fringe.push({ root, 0 });

		prev_level = 0;

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
				 
				/* Left padding. */
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