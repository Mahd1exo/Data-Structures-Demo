// src/components/HeapSection.js
import React, { useState, useEffect } from "react";
import {
  fetchHeap,
  heapInsert,
  heapExtractTop,
  heapClear, // imported clear method for the heap
} from "../services/api";
import HeapNode from "./HeapNode";
import { FaPlus, FaTrash, FaSync, FaInfoCircle } from "react-icons/fa";
import { AnimatePresence, motion } from "framer-motion";

function HeapSection() {
  const [heapArray, setHeapArray] = useState([]);
  const [inputValue, setInputValue] = useState("");
  const [errorMessage, setErrorMessage] = useState("");
  const [showInfoModal, setShowInfoModal] = useState(false);

  useEffect(() => {
    refreshHeap();
  }, []);

  async function refreshHeap() {
    try {
      const res = await fetchHeap();
      // Expecting: { heap: [ ... ] }
      setHeapArray(res.heap || []);
      setErrorMessage("");
    } catch (err) {
      console.error("Error fetching heap:", err);
      setErrorMessage("Error fetching heap data.");
    }
  }

  // Validate: must be a number (integer or float)
  function isNumeric(str) {
    return /^-?\d+(\.\d+)?$/.test(str);
  }

  async function handleInsert() {
    const val = inputValue.trim();
    if (!val) {
      setErrorMessage("Please enter a value.");
      return;
    }
    if (!isNumeric(val)) {
      setErrorMessage("Input must be a valid number (integer or float).");
      return;
    }
    // Removed uniqueness check to allow duplicate values
    setErrorMessage("");
    try {
      const res = await heapInsert(val);
      if (res.error) {
        setErrorMessage(res.error);
      } else {
        setInputValue("");
      }
      refreshHeap();
    } catch (err) {
      console.error("Heap insert error:", err);
      setErrorMessage("Error inserting into heap.");
    }
  }

  async function handleExtractTop() {
    if (heapArray.length === 0) {
      setErrorMessage("Heap is empty. Cannot extract top element.");
      return;
    }
    setErrorMessage("");
    try {
      const res = await heapExtractTop();
      if (res.error) {
        setErrorMessage(res.error);
      }
      refreshHeap();
    } catch (err) {
      console.error("Heap extract error:", err);
      setErrorMessage("Error extracting top element.");
    }
  }

  async function handleClear() {
    setErrorMessage("");
    try {
      const res = await heapClear();
      if (res.error) {
        setErrorMessage(res.error);
      }
      setInputValue("");
      refreshHeap();
    } catch (err) {
      console.error("Heap clear error:", err);
      setErrorMessage("Error clearing heap.");
    }
  }

  function toggleInfoModal() {
    setShowInfoModal((prev) => !prev);
  }

  return (
    <div className="p-6">
      {/* Header */}
      <div className="flex justify-between items-center mb-4">
        <h2 className="text-3xl font-bold">Max Heap Manager</h2>
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
              <h3 className="text-2xl font-bold">About Max Heap</h3>
              <p className="text-gray-700 text-base leading-relaxed mb-4">
                A <strong>Max Heap</strong> is a complete binary tree where every
                node's value is greater than or equal to the values of its children.
                The root holds the maximum value. This interface allows you to insert
                numeric values (integers/floats) and extract the top (maximum)
                element.
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

      {/* Controls */}
      <div className="mb-4 p-4 border rounded-md shadow-sm bg-white flex flex-wrap items-center gap-4">
        <input
          type="text"
          placeholder="Enter value"
          className="flex-grow border border-gray-300 p-2 rounded text-lg focus:outline-none focus:ring-2 focus:ring-blue-400"
          value={inputValue}
          onChange={(e) => {
            setInputValue(e.target.value);
            if (e.target.value.trim()) setErrorMessage("");
          }}
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
          onClick={handleExtractTop}
        >
          <FaTrash />
          Extract Top
        </button>
        <button
          className="bg-gray-500 text-white px-4 py-2 rounded flex items-center gap-1 hover:bg-gray-600 transition-colors duration-300"
          onClick={refreshHeap}
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

      {/* Array Representation */}
      <div className="mb-4">
        <h3 className="text-xl font-semibold text-gray-800 mb-2">
          Array Representation
        </h3>
        <div className="flex flex-wrap gap-4 justify-center">
          {heapArray.map((item, index) => (
            <div key={index} className="flex flex-col items-center">
              <div className="text-sm text-gray-500">Index: {index}</div>
              <div className="w-16 h-16 flex items-center justify-center bg-white border border-gray-300 rounded shadow">
                {item}
              </div>
            </div>
          ))}
        </div>
      </div>

      {/* Tree Display as a tree */}
      <div className="mb-2 text-sm text-gray-600">
        <strong>Max Heap Structure:</strong>
      </div>
      <div className="relative border rounded bg-gray-50 shadow-sm p-4 overflow-visible min-h-[400px]">
        {heapArray.length > 0 ? (
          <HeapNode heap={heapArray} index={0} level={0} />
        ) : (
          <p className="text-gray-500">Heap is empty</p>
        )}
      </div>
    </div>
  );
}

export default HeapSection;
