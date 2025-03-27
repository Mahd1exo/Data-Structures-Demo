import React from "react";
import {
  FaBars,             // LinkedList
  FaLayerGroup,       // Stack
  FaStream,           // Queue
  FaDatabase,         // MaxHeap
  FaSortNumericDown,  // MinHeap
  FaHashtag,          // Hash
  FaTree,             // Tree
  FaBalanceScale,     // AVL
  FaSitemap,          // BST
  FaSortAlphaDown,    // Sorted List
  FaRecycle           // Circular List
} from "react-icons/fa";

import {
  clearLinkedList,
  clearStack,
  clearQueue,
  treeClear,
  clearHash,
  bstClear,
  avlClear,
  heapClear,
  minheapClear,
  circularListClear,
  sortedListClear
} from "../services/api";

function Tabs({ activeTab, onTabChange }) {
  const tabClass = (tabName) =>
    `px-4 py-2 rounded-t ${
      activeTab === tabName
        ? "bg-white text-blue-600 font-semibold"
        : "bg-blue-500 text-white hover:bg-blue-400"
    }`;

  async function handleCleanAll() {
    const confirmed = window.confirm(
      "Are you sure you want to clean all data structures? This will remove all nodes from every data structure."
    );
    if (confirmed) {
      try {
        await Promise.all([
          clearLinkedList(),
          clearStack(),
          clearQueue(),
          treeClear(),
          clearHash(),
          bstClear(),
          avlClear(),
          heapClear(),
          minheapClear(),
          circularListClear(),
          sortedListClear()
        ]);
        alert("All data structures have been cleared.");
        // Force a re-render by temporarily clearing the active tab and then restoring it
        const currentTab = activeTab;
        onTabChange("");
        setTimeout(() => {
          onTabChange(currentTab);
        }, 100);
      } catch (err) {
        alert("Error clearing data structures. Check console for details.");
        console.error("Clean All error:", err);
      }
    }
  }

  return (
    <div className="flex space-x-1 items-center">
      <button className={tabClass("list")} onClick={() => onTabChange("list")}>
        <div className="flex items-center gap-1">
          <FaBars />
          <span>LinkedList</span>
        </div>
      </button>
      <button className={tabClass("stack")} onClick={() => onTabChange("stack")}>
        <div className="flex items-center gap-1">
          <FaLayerGroup />
          <span>Stack</span>
        </div>
      </button>
      <button className={tabClass("queue")} onClick={() => onTabChange("queue")}>
        <div className="flex items-center gap-1">
          <FaStream />
          <span>Queue</span>
        </div>
      </button>
      <button className={tabClass("sorted")} onClick={() => onTabChange("sorted")}>
        <div className="flex items-center gap-1">
          <FaSortAlphaDown />
          <span>Sorted</span>
        </div>
      </button>
      <button className={tabClass("circular")} onClick={() => onTabChange("circular")}>
        <div className="flex items-center gap-1">
          <FaRecycle />
          <span>Circular</span>
        </div>
      </button>
      <button className={tabClass("heap")} onClick={() => onTabChange("heap")}>
        <div className="flex items-center gap-1">
          <FaDatabase />
          <span>MaxHeap</span>
        </div>
      </button>
      <button className={tabClass("minheap")} onClick={() => onTabChange("minheap")}>
        <div className="flex items-center gap-1">
          <FaSortNumericDown />
          <span>MinHeap</span>
        </div>
      </button>
      <button className={tabClass("hash")} onClick={() => onTabChange("hash")}>
        <div className="flex items-center gap-1">
          <FaHashtag />
          <span>Hash</span>
        </div>
      </button>
      <button className={tabClass("tree")} onClick={() => onTabChange("tree")}>
        <div className="flex items-center gap-1">
          <FaTree />
          <span>Tree</span>
        </div>
      </button>
      <button className={tabClass("bst")} onClick={() => onTabChange("bst")}>
        <div className="flex items-center gap-1">
          <FaSitemap />
          <span>BST</span>
        </div>
      </button>
      <button className={tabClass("avl")} onClick={() => onTabChange("avl")}>
        <div className="flex items-center gap-1">
          <FaBalanceScale />
          <span>AVL</span>
        </div>
      </button>
      {/* Extra "Clean All" tab */}
      <button
        className="px-4 py-2 rounded-t bg-red-500 text-white hover:bg-red-400 font-semibold ml-auto"
        onClick={handleCleanAll}
      >
        Clean All
      </button>
    </div>
  );
}

export default Tabs;
