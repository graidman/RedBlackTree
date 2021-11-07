#include<iostream>
using namespace std;

#define RED 0
#define BLACK 1
typedef int KEY_TYPE;


//定义红黑树的结点
typedef struct _rbtree_node {
	unsigned char color;//定义颜色
	struct _rbtree_node* left;//左子树
	struct _rbtree_node* right;//右字数
	struct _rbtree_node* parent;//父结点

	KEY_TYPE key;
	void* value;
}rbtree_node;

//定义红黑树
typedef struct _rbtree {
	rbtree_node* root;//根结点
	rbtree_node* ni1;//叶子节点
}rbtree;

rbtree_node* rbtree_mini(rbtree* T, rbtree_node* x) {//找到最小的结点
	while (x->left != T->ni1) {
		x = x->left;
	}
	return x;
}

//找到最大的叶子结点
rbtree_node* rbtree_maxi(rbtree* T, rbtree_node* x) {
	while (x->right != T->ni1) {
		x = x->right;
	}
	return x;
}

rbtree_node* rbtree_successor(rbtree* T, rbtree_node* x) {
	rbtree_node* y = x->parent;
	if (x->right != T->ni1) {
		return rbtree_mini(T, x->right);
	}
	while ((y != T->ni1) && (x == y->right)) {
		x = y;
		y = y->parent;
	}
	return y;
}

//红黑树左旋
//传入红黑树和结点
void _left_rotate(rbtree* T, rbtree_node* x) {
	rbtree_node* y = x->right;

	x->right = y->left;
	if (y->left != T->ni1) {
		y->left->parent = x;
	}

	y->parent = x->parent;
	if (x->parent == T->ni1) {
		T->root = y;
	}
	else if (x == x->parent->left) {
		x->parent->left = y;
	}
	else {
		x->parent->right = y;
	}

	y->left = x;
	x->parent = y;
}

//红黑树右旋
void _right_rotate(rbtree* T, rbtree_node* y) {
	rbtree_node* x = y->left;

	y->left = x->right;
	if (x->right != T->ni1) {
		x->right->parent = y;
	}

	x->parent = y->parent;
	if (y->parent == T->ni1) {
		T->root = x;
	}
	else if (y == y->parent->right) {
		y->parent->right = x;
	}
	else {
		y->parent->right = x;
	}

	x->right = y;
	y->parent = x;
}
//红黑树插入后调整
void rbtree_insert_fixup(rbtree* T,rbtree_node* z) {
	while(z->parent->color == RED) {
		if (z->parent == z->parent->parent->left) {
			rbtree_node* y = z->parent->parent->right;
			if (y->color == RED) {//如果叔父结点为祖父的左子树且为红色,将父亲和叔父改为黑，祖父为红
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;

				z = z->parent->parent;
			}
			else {//如果叔父为空
				if (z == z->parent->right) {
					z = z->parent;
					_left_rotate(T, z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				_right_rotate(T, z->parent->parent);
			}
		}
		else {//如果叔父是左子树
			rbtree_node* y = z->parent->parent->left;
			if (y->color == RED) {
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;

				z = z->parent->parent;//返回祖父结点
			}
			else {
				if (z == z->parent->left) {
					z = z->parent;
					_right_rotate(T, z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				_left_rotate(T, z->parent->parent);
			}
		}
		
	}
	T->root->color = BLACK;
}
//红黑树插入
void rbtree_insert(rbtree* T, rbtree_node* z) {
	rbtree_node* x = T->root;
	rbtree_node* y = T->ni1;

	while (x != T->ni1) {
		y = x;
		if (z->key < x->key) {
			x = x->left;
		}
		else if (z->key > x->key) {
			x = x->right;
		}
		else {
			return;
		}
	}
	z->parent = y;
	if (y == T->ni1) {
		T->root = z;
	}
	else if (z->key < y->key) {
		y->left = z;
	}
	else {
		y->right = z;
	}
	z->left = T->ni1;
	z->right = T->ni1;

	z->color = RED;
	rbtree_insert_fixup(T, z);
}

//红黑树删除调整
void rbtree_delete_fixup(rbtree* T, rbtree_node* x) {
	while ((x != T->root) && (x->color == BLACK)) {
		if (x == x->parent->left) {
			rbtree_node* w = x->parent->right;
			if (w->color == RED) {
				x->parent->color = RED;

				_left_rotate(T, x->parent);
				w = x->parent->right;
			}

			if ((w->left->color == BLACK) && (w->right->color == BLACK)) {
				w->color = RED;
				x = x->parent;
			}
			else {
				if (w->right->color == BLACK) {
					w->left->color = BLACK;
					w->color = RED;
					_right_rotate(T, w);
					w = x->parent->right;
				}

				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				_left_rotate(T, x->parent);
				x = T->root;
			}
		}
		else {
			rbtree_node* w = x->parent->left;
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				_right_rotate(T, x->parent);
				w = x->parent->left;
			}

			if ((w->left->color == BLACK) && (w->right->color == BLACK)) {
				w->color = RED;
				x = x->parent;
			}
			else {
				if (w->left->color == BLACK) {
					w->right->color = BLACK;
					w->color = RED;
					_left_rotate(T, w);
					w = x->parent->left;
				}
				
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				_right_rotate(T, x->parent);

				x = T->root;
			}
		}
	}
	x->color = BLACK;
}

//红黑树删除
rbtree_node* rbtree_delete(rbtree* T, rbtree_node* z) {
	rbtree_node* y = T->ni1;
	rbtree_node* x = T->ni1;

	if ((z->left == T->ni1) || (z->right == T->ni1)) {
		y = z;
	}
	else {
		y = rbtree_successor(T, z);
	}

	if (y->left != T->ni1) {
		x = y->left;
	}
	else if (y->right != T->ni1) {
		x = y->right;
	}

	x->parent = y->parent;
	if (y->parent == T->ni1) {
		T->root = x;
	}
	else if (y == y->parent->left) {
		y->parent->left = x;
	}
	else {
		y->parent->right = x;
	}

	if (y != z) {
		z->key = y->key;
		z->value = y->value;
	}

	if (y->color == BLACK) {
		rbtree_delete_fixup(T, x);
	}
	return y;
}

rbtree_node* rbtree_search(rbtree* T, KEY_TYPE key) {
	rbtree_node* node = T->root;
	while (node != T->ni1) {
		if (key < node->key) {
			node = node->left;
		}
		else if (key > node->key) {
			node = node->right;
		}
		else
		{
			return node;
		}
	}
	return T->ni1;
}

void rbtree_traversal(rbtree* T, rbtree_node* node) {
	if (node != T->ni1) {
		rbtree_traversal(T, node->left);
		cout << "key:" << node->key << " " << "color:" << node->color<<endl;
		rbtree_traversal(T, node->right);
	}
}

int main() {

	int keyArray[20] = { 24,25,13,35,23, 26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15 };

	rbtree* T = (rbtree*)malloc(sizeof(rbtree));
	if (T == NULL) {
		printf("malloc failed\n");
		return -1;
	}

	T->ni1 = (rbtree_node*)malloc(sizeof(rbtree_node));
	T->ni1->color = BLACK;
	T->root = T->ni1;

	rbtree_node* node = T->ni1;
	int i = 0;
	for (i = 0; i < 20; i++) {
		node = (rbtree_node*)malloc(sizeof(rbtree_node));
		node->key = keyArray[i];
		node->value = NULL;

		rbtree_insert(T, node);

	}

	rbtree_traversal(T, T->root);
	printf("----------------------------------------\n");

	for (i = 0; i < 20; i++) {

		rbtree_node* node = rbtree_search(T, keyArray[i]);
		rbtree_node* cur = rbtree_delete(T, node);
		free(cur);

		rbtree_traversal(T, T->root);
		printf("----------------------------------------\n");
	}



}