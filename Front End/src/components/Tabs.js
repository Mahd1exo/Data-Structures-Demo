// src/components/Tabs.js
import React from "react";
import {
  FaBars,       // LinkedList
  FaLayerGroup, // Stack
  FaStream,     // Queue
  FaDatabase,   // Heap
  FaHashtag,    // Hash
  FaTree,       // Tree
  FaSitemap     // BST
} from "react-icons/fa";

function Tabs({ activeTab, onTabChange }) {
  const tabClass = (tabName) =>
    `px-4 py-2 rounded-t ${
      activeTab === tabName
        ? "bg-white text-blue-600 font-semibold"
        : "bg-blue-500 text-white hover:bg-blue-400"
    }`;

  return (
    <div className="flex space-x-1">
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
      <button className={tabClass("heap")} onClick={() => onTabChange("heap")}>
        <div className="flex items-center gap-1">
          <FaDatabase />
          <span>Heap</span>
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
    </div>
  );
}

export default Tabs;
