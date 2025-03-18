// src/components/TreeSection.js
import React, { useRef, useState, useEffect } from "react";
import { fetchTree, treeInsert, treeRemove, treeInsertChild } from "../services/api";
import TreeNode from "./TreeNode";
import { AnimatePresence, motion } from "framer-motion";
import { FaPlus, FaTrash, FaSync, FaInfoCircle } from "react-icons/fa";

function TreeSection() {
  const [tree, setTree] = useState(null);
  const [insertValue, setInsertValue] = useState("");
  const [insertParentId, setInsertParentId] = useState("");
  const [removeId, setRemoveId] = useState("");
  const [errorMessage, setErrorMessage] = useState("");
  const [showInfoModal, setShowInfoModal] = useState(false);

  // We store a ref to the tree container so we can measure absolute positions.
  const treeContainerRef = useRef(null);

  useEffect(() => {
    fetchTreeData();
  }, []);

  async function fetchTreeData() {
    try {
      const res = await fetchTree();
      setTree(res.tree || null);
    } catch (err) {
      console.error("Tree fetch error:", err);
      setErrorMessage("Error fetching tree data.");
    }
  }

  async function handleInsert() {
    if (!insertValue.trim()) {
      setErrorMessage("Please enter a valid node value.");
      return;
    }
    if (insertParentId.trim() && isNaN(insertParentId.trim())) {
      setErrorMessage("Parent ID must be a valid number.");
      return;
    }
    setErrorMessage("");
    try {
      if (insertParentId.trim()) {
        await treeInsertChild(parseInt(insertParentId.trim(), 10), insertValue.trim());
      } else {
        await treeInsert(insertValue.trim());
      }
      setInsertValue("");
      setInsertParentId("");
      fetchTreeData();
    } catch (err) {
      console.error("Tree insert error:", err);
      setErrorMessage("Error inserting node.");
    }
  }

  async function handleRemove() {
    if (!removeId.trim()) {
      setErrorMessage("Please enter a valid node ID to remove.");
      return;
    }
    if (isNaN(removeId.trim())) {
      setErrorMessage("Node ID must be a valid number.");
      return;
    }
    setErrorMessage("");
    try {
      await treeRemove(parseInt(removeId.trim(), 10));
      setRemoveId("");
      fetchTreeData();
    } catch (err) {
      console.error("Tree remove error:", err);
      setErrorMessage("Error removing node.");
    }
  }

  function toggleInfoModal() {
    setShowInfoModal((prev) => !prev);
  }

  return (
    <div className="w-full p-6">
      {/* Header */}
      <div className="flex justify-between items-center mb-4">
        <h2 className="text-3xl font-bold text-gray-800">Tree Manager</h2>
        <button
          onClick={toggleInfoModal}
          className="flex items-center gap-1 text-gray-600 hover:text-gray-800 transition-colors duration-300"
        >
          <FaInfoCircle className="text-2xl" />
        </button>
      </div>

      {/* Global Error Message */}
      {errorMessage && (
        <div className="mb-4 p-2 border border-red-300 rounded text-red-600 text-sm">
          {errorMessage}
        </div>
      )}

      {/* Info Modal */}
      <AnimatePresence>
        {showInfoModal && (
          <motion.div
            className="fixed inset-0 flex items-center justify-center z-50"
            initial={{ opacity: 0 }}
            animate={{ opacity: 1 }}
            exit={{ opacity: 0 }}
          >
            <div
              className="absolute inset-0 bg-black opacity-40"
              onClick={toggleInfoModal}
            ></div>
            <motion.div
              className="relative bg-white rounded-xl p-6 max-w-xl mx-auto z-10 shadow-2xl"
              initial={{ scale: 0.8, y: -50 }}
              animate={{ scale: 1, y: 0 }}
              exit={{ scale: 0.8, y: 50 }}
              transition={{ duration: 0.3 }}
            >
              <h3 className="text-2xl font-bold mb-4 text-gray-800">About Trees</h3>
              <p className="text-gray-700 text-base leading-relaxed mb-4">
                A <strong>tree</strong> is a hierarchical data structure that
                represents relationships in a parent-child form...
              </p>
              <button
                onClick={toggleInfoModal}
                className="bg-indigo-600 hover:bg-indigo-700 text-white px-4 py-2 rounded transition-colors duration-300"
              >
                Close
              </button>
            </motion.div>
          </motion.div>
        )}
      </AnimatePresence>

      {/* Insertion Section */}
      <div className="mb-4 p-4 border rounded-md shadow-sm bg-white">
        <h3 className="text-lg font-medium text-gray-700 border-b pb-1 mb-3">Insert Node</h3>
        <div className="flex items-center gap-4">
          <input
            type="text"
            placeholder="Enter node value"
            className="flex-1 border border-gray-300 p-2 rounded focus:outline-none focus:ring focus:ring-blue-300"
            value={insertValue}
            onChange={(e) => setInsertValue(e.target.value)}
          />
          <input
            type="text"
            placeholder="Parent ID (optional)"
            className="flex-1 border border-gray-300 p-2 rounded focus:outline-none focus:ring focus:ring-blue-300 max-w-xs"
            value={insertParentId}
            onChange={(e) => setInsertParentId(e.target.value)}
          />
          <button
            className="flex items-center gap-1 bg-blue-500 hover:bg-blue-600 text-white px-4 py-2 rounded text-sm font-semibold transition-colors duration-300"
            onClick={handleInsert}
          >
            <FaPlus />
            Insert
          </button>
        </div>
      </div>

      {/* Removal Section */}
      <div className="mb-4 p-4 border rounded-md shadow-sm bg-white">
        <h3 className="text-lg font-medium text-gray-700 border-b pb-1 mb-3">Remove Node</h3>
        <div className="flex items-center gap-4">
          <input
            type="text"
            placeholder="Enter node ID to remove"
            className="flex-1 border border-gray-300 p-2 rounded focus:outline-none focus:ring focus:ring-red-300 max-w-xs"
            value={removeId}
            onChange={(e) => setRemoveId(e.target.value)}
          />
          <button
            className="flex items-center gap-1 bg-red-500 hover:bg-red-600 text-white px-4 py-2 rounded text-sm font-semibold transition-colors duration-300"
            onClick={handleRemove}
          >
            <FaTrash />
            Remove by ID
          </button>
          <button
            className="flex items-center gap-1 bg-gray-500 hover:bg-gray-600 text-white px-4 py-2 rounded text-sm font-semibold transition-colors duration-300"
            onClick={fetchTreeData}
          >
            <FaSync className="animate-spin" />
            Refresh
          </button>
        </div>
      </div>

      {/* Tree Display */}
      <div className="mb-2 text-sm text-gray-600">
        <strong>Tree Structure:</strong>
      </div>
      {/* The container for the entire tree */}
      <div
        className="relative border rounded bg-gray-50 shadow-sm p-4 overflow-visible"
        ref={treeContainerRef}
        style={{ minHeight: "400px" }} // for demonstration
      >
        {tree ? (
          <TreeNode
            node={tree}
            index={0}
            level={0}
            parentId=""
            treeContainerRef={treeContainerRef}
          />
        ) : (
          <p className="text-gray-500">Tree is empty</p>
        )}
      </div>
    </div>
  );
}

export default TreeSection;
