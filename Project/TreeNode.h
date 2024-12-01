#pragma once

#include <vector>
#include <memory>

namespace TMF
{
	template <typename T>
	class TreeNode
	{
	public:
		T m_nodeData;
		std::shared_ptr<TreeNode<T>> m_pParent{};
		std::vector<std::shared_ptr<TreeNode<T>>> m_children;

		void AddChild(std::shared_ptr<TreeNode<T>> child)
		{
			m_children.push_back(std::move(child));
		}

		TreeNode();
		~TreeNode();


		// ŠJ•úˆ—
		void Finalize()
		{
			//for (auto& child : m_children)
			//{
			//	child->
			//}
		}
	};
	template<typename T>
	inline TreeNode<T>::TreeNode()
	{
	}
	template<typename T>
	inline TreeNode<T>::~TreeNode()
	{
	}
}