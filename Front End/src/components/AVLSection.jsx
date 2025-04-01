// src/components/AVLSection.js
import React, { useState, useEffect, useRef } from "react";
import {
  fetchAVL,
  avlInsert,
  avlRemove,
  avlClear, // <-- import the clear function for AVL
} from "../services/api";
import AVLNode from "./AVLNode";
import { AnimatePresence, motion } from "framer-motion";
import { FaPlus, FaTrash, FaSync, FaInfoCircle } from "react-icons/fa";

function AVLSection() {
  const [bst, setBst] = useState(null);
  const [nodeValue, setNodeValue] = useState("");
  const [errorMessage, setErrorMessage] = useState("");
  const [showInfoModal, setShowInfoModal] = useState(false);
  const avlContainerRef = useRef(null);

  useEffect(() => {
    fetchAVLData();
  }, []);

  async function fetchAVLData() {
    try {
      const res = await fetchAVL();
      // Expecting a structure: { bst: { ... } }
      if (res.error) {
        setErrorMessage(res.error);
      } else {
        setBst(res.bst || null);
        setErrorMessage("");
      }
    } catch (err) {
      console.error("AVL fetch error:", err);
      setErrorMessage("Error fetching AVL data.");
    }
  }

  // Recursively check if the value already exists in the tree.
  // Convert the input value to an integer to ensure proper comparison.
  function existsInTree(node, value) {
    if (!node) return false;
    const intValue = parseInt(value, 10);
    if (node.value === intValue) return true;
    return existsInTree(node.left, intValue) || existsInTree(node.right, intValue);
  }

  async function handleInsert() {
    const trimmedValue = nodeValue.trim();
    // Validate non-empty input
    if (!trimmedValue) {
      setErrorMessage("Please enter a valid node value.");
      return;
    }
    // Validate integer (allows negatives)
    if (!/^-?\d+$/.test(trimmedValue)) {
      setErrorMessage("Input must be a valid integer.");
      return;
    }
    // Validate uniqueness
    if (bst && existsInTree(bst, trimmedValue)) {
      setErrorMessage("Value already exists in the tree.");
      return;
    }
    setErrorMessage("");
    try {
      const res = await avlInsert(trimmedValue);
      if (res.error) {
        setErrorMessage(res.error);
      } else {
        setNodeValue("");
      }
      fetchAVLData();
    } catch (err) {
      console.error("AVL insert error:", err);
      setErrorMessage("Error inserting node.");
    }
  }

  async function handleRemove() {
    const trimmedValue = nodeValue.trim();
    if (!trimmedValue) {
      setErrorMessage("Please enter a valid node value to remove.");
      return;
    }
    // Validate integer format
    if (!/^-?\d+$/.test(trimmedValue)) {
      setErrorMessage("Input must be a valid integer.");
      return;
    }
    // Check if the value exists in the tree (using integer comparison)
    if (!bst || !existsInTree(bst, trimmedValue)) {
      setErrorMessage("Value does not exist in the tree.");
      return;
    }
    setErrorMessage("");
    try {
      const res = await avlRemove(trimmedValue);
      if (res.error) {
        setErrorMessage(res.error);
      } else {
        setNodeValue("");
      }
      fetchAVLData();
    } catch (err) {
      console.error("AVL remove error:", err);
      setErrorMessage("Error removing node.");
    }
  }

  async function handleClear() {
    setErrorMessage("");
    try {
      const res = await avlClear();
      if (res.error) {
        setErrorMessage(res.error);
      } else {
        setBst(null);
        setNodeValue("");
      }
    } catch (err) {
      console.error("AVL clear error:", err);
      setErrorMessage("Error clearing AVL tree.");
    }
  }

  function toggleInfoModal() {
    setShowInfoModal((prev) => !prev);
  }

  return (
    <div className="w-full p-6">
      {/* Header */}
      <div className="flex justify-between items-center mb-4">
        <h2 className="text-3xl font-bold text-gray-800">AVL Tree Manager</h2>
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
              <h3 className="text-2xl font-bold mb-4 text-gray-800">
                About AVL Trees
              </h3>
              <p className="text-gray-700 text-base leading-relaxed mb-4">
                An AVL tree is a self-balancing binary search tree. In our JSON
                structure, each node has a unique <strong>id</strong>, a{" "}
                <strong>value</strong>, and pointers to the{" "}
                <strong>left</strong> and <strong>right</strong> children.
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

      {/* Control Panel */}
      <div className="mb-4 p-4 border rounded-md shadow-sm bg-white flex flex-col gap-4">
        <div className="flex items-center gap-4 w-full">
          <input
            type="text"
            placeholder="Enter node value"
            className="flex-1 border border-gray-300 p-3 rounded text-lg focus:outline-none focus:ring focus:ring-blue-300"
            value={nodeValue}
            onChange={(e) => setNodeValue(e.target.value)}
          />
          <button
            className="bg-blue-500 text-white px-4 py-2 rounded flex items-center gap-1 hover:bg-blue-600 transition-colors duration-300"
            onClick={handleInsert}
          >
            <FaPlus />
            Insert
          </button>
          <button
            className="bg-red-500 text-white px-4 py-2 rounded flex items-center gap-1 hover:bg-red-600 transition-colors duration-300"
            onClick={handleRemove}
          >
            <FaTrash />
            Remove
          </button>
          <button
            className="bg-gray-500 text-white px-4 py-2 rounded flex items-center gap-1 hover:bg-gray-600 transition-colors duration-300"
            onClick={fetchAVLData}
          >
            <FaSync className="animate-spin" />
            Refresh
          </button>
          <button
            className="bg-gray-500 text-white px-4 py-2 rounded flex items-center gap-1 hover:bg-gray-600 transition-colors duration-300"
            onClick={handleClear}
          >
            <FaTrash className="rotate-180" />
            Clear
          </button>
        </div>
      </div>

      {/* AVL Tree Display */}
      <div className="mb-4 text-sm text-gray-600">
        <strong>AVL Tree Structure:</strong>
      </div>
      <div
        className="relative border rounded bg-gray-50 shadow-sm p-4 overflow-auto min-h-[400px]"
        ref={avlContainerRef}
      >
        {bst ? (
          <AVLNode node={bst} level={0} />
        ) : (
          <p className="text-gray-500">AVL tree is empty.</p>
        )}
      </div>
    </div>
  );
}

export default AVLSection;
