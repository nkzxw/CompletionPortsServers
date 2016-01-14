#pragma once
#include <stdexcept>
#include <iostream>

namespace BT
{
	template <typename T>
	struct Node
	{
		T data_;
		long count_;
		Node* left_;
		Node* right_;

		//Node is not default constructible
		Node() = delete;

		Node(T& in) :
			data_(in),
			count_(1),
			left_(nullptr),
			right_(nullptr)
		{}

		Node(const Node<T>& in)
			: data_(in.data_)
			, count_(in.count)
		{
			if (in.left_ != nullptr)
				left_ = new Node<T>(*in.left_);
			if (in.right_ != nullptr)
				right_ = new Node<T>(*in.right_);
		}

		Node(Node&& old) :
			data_(std::move(old.data_)),
			count_(old.count_)
		{
			if (in.left_ != nullptr)
			{
				left_ = new Node<T>(*in.left_);
				delete old.left_;
			}
			if (in.right_ != nullptr)
			{
				right_ = new Node<T>(*in.right_);
				delete old.right_;
			}

			old.left_ = nullptr;
			old.right_ = nullptr;
		}

		~Node()
		{
			if (left_)
			{
				delete left_;
			}
			if (right_)
			{
				delete right_;
			}
		}
	};

	//if locking is enabled, we will use mutex to protect access to the data
	template <typename T, bool locking = false>
	class BinaryTree
	{
	public:
		typedef Node<T>* pointer_type;
	public:
		BinaryTree()
			: head_(nullptr),
			level_(0),
			average_(0.0),
			count_(0),
			sum_(0)
		{
		}

		BinaryTree(T& data) :
			head_(new Node<T>(data)),
			level_(1)
		{
		}

		BinaryTree(BinaryTree&& data)
			: level_(data.level_),
			count_(data.count_),
			sum_(data.sum_),
			average_(data.average_)
		{
			if (data.head_ != nullptr)
			{
				head_ = new Node<T>(*data.head_);
				delete data.head_;
			}
			data.head_ = nullptr;
		}

		~BinaryTree()
		{
			//this will recursively delete all nodes
			delete head_;
		}

		void add(T data)
		{
			if (head_ == nullptr)
			{
				head_ = new Node<T>(data);
				level_ = 1;
			}
			else
			{
				auto level(1);
				_add(data, head_, level);
				if (level > level_)
					level_ = level;
			}
			_update_average(data);
		}

		void remove(T data)
		{
			//TODO: implement
		}

		//make the tree balanced
		void rebalance()
		{
			//TODO: implement
		}

		void print() const
		{
			if (head_ != nullptr)
			{
				_print(head_);
			}
			else
			{
				std::cout << "Tree is empty" << std::endl;
			}
		}

		//prints tree level after level starting from one
		void printNice() const
		{
			for (auto i = 1; i <= level_; ++i)
			{
				_print_level(head_, 1, i);
				std::cout << std::endl;
			}
		}

		int treeDepth() const { return level_; }

		double treeAverage() const { return average_; }

	private:

		//recursive implementation
		//if tree is massive we could run into problem of running out of stack space
		int _add(T data, pointer_type node, int& level)
		{
			if (data < node->data_)
			{
				if (node->left_ == nullptr)
				{
					node->left_ = new Node<T>(data);
					if (node->right_ == nullptr)
					{
						++level;
					}
				}
				else
				{
					_add(data, node->left_, ++level);
				}
			}
			else if (data > node->data_)
			{
				if (node->right_ == nullptr)
				{
					node->right_ = new Node<T>(data);
					if (node->left_ == nullptr)
					{
						++level;
					}
				}
				else
				{
					_add(data, node->right_, ++level);
				}
			}
			else
			{
				++node->count_;
			}

			return level;
		}

		void _print(pointer_type node) const
		{
			bool printed(false);
			if (node->left_ != nullptr)
			{
				_print(node->left_);
			}
			else
			{
				std::cout << node->data_ << std::endl;
				printed = true;
			}

			if (node == head_)
			{
				std::cout << node->data_ << std::endl;
				printed = true;
			}

			if (node->right_ != nullptr)
			{
				_print(node->right_);
			}
			else
			{
				if (!printed)
				{
					std::cout << node->data_ << std::endl;
					printed = true;
				}
			}

			if (!printed)
				std::cout << node->data_ << std::endl;
		}

		void _print_level(pointer_type node, int current_level, int required_level) const
		{
			if (current_level > required_level)
				return;
			else if (current_level == required_level)
			{
				std::cout << node->data_ << " ";
				return;
			}

			if (node->left_ != nullptr)
			{
				_print_level(node->left_, current_level + 1, required_level);
			}

			if (node->right_ != nullptr)
			{
				_print_level(node->right_, current_level + 1, required_level);
			}
		}

		void _update_average(const T& data)
		{
			++count_;
			sum_ += data;
			average_ = sum_ / (count_ * 1.0);
		}

		//data definiotions go here
		pointer_type head_;

		int level_;

		//number of items in the tree
		int count_;
		long sum_;

		double average_;
	};

}