// src/components/BSTSection.js
import React, { useState, useEffect, useRef } from "react";
import { fetchBST, bstInsert, bstRemove, bstClear } from "../services/api";
import BSTNode from "./BSTNode";
import { AnimatePresence, motion } from "framer-motion";
import { FaPlus, FaTrash, FaSync, FaInfoCircle } from "react-icons/fa";

function BSTSection() {
  const [bst, setBST] = useState(null);
  const [nodeValue, setNodeValue] = useState("");
  const [errorMessage, setErrorMessage] = useState("");
  const [showInfoModal, setShowInfoModal] = useState(false);
  const bstContainerRef = useRef(null);

  useEffect(() => {
    fetchBSTData();
  }, []);

  // Helper function: Validate integer (allows optional leading -)
  function isValidInt(str) {
    return /^-?\d+$/.test(str);
  }

  // Helper function: Recursively check if the value already exists in the BST
  function existsInBST(node, value) {
    if (!node) return false;
    if (node.value === value) return true;
    return existsInBST(node.left, value) || existsInBST(node.right, value);
  }

  async function fetchBSTData() {
    try {
      const res = await fetchBST();
      if (res.error) {
        setErrorMessage(res.error);
      } else {
        setBST(res.bst || null);
        setErrorMessage("");
      }
    } catch (err) {
      console.error("BST fetch error:", err);
      setErrorMessage("Error fetching BST data.");
    }
  }

  async function handleInsert() {
    const trimmedValue = nodeValue.trim();
    if (!trimmedValue) {
      setErrorMessage("Please enter a valid integer.");
      return;
    }
    if (!isValidInt(trimmedValue)) {
      setErrorMessage("Input must be an integer (no special characters).");
      return;
    }
    if (bst && existsInBST(bst, trimmedValue)) {
      setErrorMessage("Value already exists in the tree.");
      return;
    }
    setErrorMessage("");
    try {
      const res = await bstInsert(trimmedValue);
      if (res.error) {
        setErrorMessage(res.error || "Cannot add duplicate value.");
      } else {
        setNodeValue("");
      }
      fetchBSTData();
    } catch (err) {
      console.error("BST insert error:", err);
      setErrorMessage("Error inserting node.");
    }
  }

  async function handleRemove() {
    const trimmedValue = nodeValue.trim();
    if (!trimmedValue) {
      setErrorMessage("Please enter a valid integer to remove.");
      return;
    }
    if (!isValidInt(trimmedValue)) {
      setErrorMessage("Input must be an integer (no special characters).");
      return;
    }
    // Validate deletion: Check if the value exists in the BST
    if (!bst || !existsInBST(bst, trimmedValue)) {
      setErrorMessage("Value does not exist in the tree.");
      return;
    }
    setErrorMessage("");
    try {
      const res = await bstRemove(trimmedValue);
      if (res.error) {
        setErrorMessage(res.error || "Node not found.");
      } else {
        setNodeValue("");
      }
      fetchBSTData();
    } catch (err) {
      console.error("BST remove error:", err);
      setErrorMessage("Error removing node.");
    }
  }

  async function handleClear() {
    setErrorMessage("");
    try {
      const res = await bstClear();
      if (res.error) {
        setErrorMessage(res.error);
      } else {
        // Clear local state
        setBST(null);
        setNodeValue("");
      }
    } catch (err) {
      console.error("BST clear error:", err);
      setErrorMessage("Error clearing BST.");
    }
  }

  function toggleInfoModal() {
    setShowInfoModal((prev) => !prev);
  }

  return (
    <div className="w-full p-6">
      {/* Header */}
      <div className="flex justify-between items-center mb-4">
        <h2 className="text-3xl font-bold text-gray-800">BST Manager</h2>
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
                About BST
              </h3>
              <p className="text-gray-700 text-base leading-relaxed mb-4">
                A <strong>Binary Search Tree (BST)</strong> is a data structure
                where for every node, values in the left subtree are less than
                the node’s value and values in the right subtree are greater.
                Duplicate values are not allowed, and removing a value that
                doesn’t exist returns an error.
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
      <div className="mb-4 p-4 border rounded-md shadow-sm bg-white">
        <div className="flex flex-wrap items-center gap-4">
          <input
            type="text"
            placeholder="Enter integer value"
            className="flex-1 border border-gray-300 p-2 rounded focus:outline-none focus:ring focus:ring-blue-300"
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
            onClick={fetchBSTData}
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

      {/* BST Display */}
      <div className="mb-2 text-sm text-gray-600">
        <strong>BST Structure:</strong>
      </div>
      <div
        className="relative border rounded bg-gray-50 shadow-sm p-4 overflow-visible"
        ref={bstContainerRef}
        style={{ minHeight: "400px" }}
      >
        {bst ? (
          <BSTNode node={bst} level={0} />
        ) : (
          <p className="text-gray-500">BST is empty</p>
        )}
      </div>
    </div>
  );
}

export default BSTSection;
