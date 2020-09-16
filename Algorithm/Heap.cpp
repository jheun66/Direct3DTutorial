#include "Framework.h"

Heap::Heap()
{
}

Heap::~Heap()
{
}

void Heap::Insert(Node* node)
{
    heap.emplace_back(node);
    UpdateUpper(heap.size() - 1);
}

void Heap::UpdateUpper(int index)
{
    int curIndex = index;
    int parent = (curIndex - 1) / 2;

    while (curIndex != 0)
    {
        if (heap[parent]->f < heap[curIndex]->f)
            break;

        Swap(curIndex, parent);

        curIndex = parent;
        parent = (curIndex - 1) / 2;
    }
}

Node* Heap::DeleteRoot()
{
    Node* root = heap.front();
    Swap(0, heap.size() - 1);
    heap.pop_back();
    UpdateLower(0);

    return root;
}

void Heap::UpdateLower(int index)
{
    int curIndex = index;
    int lChild = curIndex * 2 + 1;
    int rChild = curIndex * 2 + 2;

    int minNode = curIndex;
    while (true)
    {
        if (lChild < heap.size() && heap[lChild]->f < heap[minNode]->f)
            minNode = lChild;

        if (rChild < heap.size() && heap[rChild]->f < heap[minNode]->f)
            minNode = rChild;

        if (minNode == curIndex)
            break;

        Swap(curIndex, minNode);
        curIndex = minNode;
        lChild = curIndex * 2 + 1;
        rChild = curIndex * 2 + 2;
    }

}

void Heap::Swap(int n1, int n2)
{
    Node* temp = heap[n1];
    heap[n1] = heap[n2];
    heap[n2] = temp;
}

void Heap::Clear()
{
    heap.clear();
}