// src/components/SortedListSection.js
import React, { useState, useEffect } from "react";
import {
  fetchSortedList,
  sortedListInsert,
  sortedListRemove,
  sortedListClear,
} from "../services/api";
import { AnimatePresence, motion } from "framer-motion";
import {
  FaPlus,
  FaTrash,
  FaSync,
  FaInfoCircle,
  FaArrowRight,
} from "react-icons/fa";

// Generate a unique ID for local mapping (for key purposes)
function genId() {
  return Date.now().toString(36) + Math.random().toString(36).substring(2);
}

function SortedListSection() {
  const [list, setList] = useState([]);
  const [inputValue, setInputValue] = useState("");
  const [errorMessage, setErrorMessage] = useState("");
  const [showInfoModal, setShowInfoModal] = useState(false);

  // Fetch the sorted list on mount
  useEffect(() => {
    handleFetch();
    // eslint-disable-next-line
  }, []);

  async function handleFetch() {
    try {
      const data = await fetchSortedList(); // expects { sortedList: ["val1", "val2", ...] }
      const rawArray = data.sortedList || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setList(mapped);
      setErrorMessage("");
    } catch (err) {
      console.error("Error fetching sorted list:", err);
      setErrorMessage("Error fetching sorted list.");
    }
  }

  async function handleInsert() {
    const value = inputValue.trim();
    if (!value) {
      setErrorMessage("Please enter a valid value to insert.");
      return;
    }
    if (value.length > 16) {
      setErrorMessage("Value must be 16 characters or less.");
      return;
    }
    setErrorMessage("");
    try {
      const data = await sortedListInsert(value);
      const rawArray = data.sortedList || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setList(mapped);
      setInputValue("");
    } catch (err) {
      console.error("Error inserting into sorted list:", err);
      setErrorMessage("Error inserting value.");
    }
  }

  async function handleRemove() {
    const value = inputValue.trim();
    if (!value) {
      setErrorMessage("Please enter a valid value to remove.");
      return;
    }
    setErrorMessage("");
    try {
      const data = await sortedListRemove(value);
      const rawArray = data.sortedList || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setList(mapped);
      setInputValue("");
    } catch (err) {
      console.error("Error removing from sorted list:", err);
      setErrorMessage("Error removing value.");
    }
  }

  async function handleClear() {
    setErrorMessage("");
    try {
      const data = await sortedListClear();
      const rawArray = data.sortedList || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setList(mapped);
    } catch (err) {
      console.error("Error clearing sorted list:", err);
      setErrorMessage("Error clearing the list.");
    }
  }

  function toggleInfoModal() {
    setShowInfoModal((prev) => !prev);
  }

  return (
    <div className="w-full p-6">
      {/* Header: Title and Info Button */}
      <div className="flex justify-between items-center mb-4">
        <h2 className="text-3xl font-bold text-gray-800">Sorted Linked List</h2>
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
                About Sorted Lists
              </h3>
              <p className="text-gray-700 text-base leading-relaxed mb-4">
                A <strong>sorted linked list</strong> maintains its elements in sorted order.
                Insertions are placed automatically in the correct order, and removal
                operations update the list accordingly.
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

      {/* Control Row in Single Row */}
      <div className="mb-4 p-4 border rounded-md shadow-sm bg-white flex items-center gap-4">
        <input
          type="text"
          placeholder="Enter value"
          className="flex-1 border border-gray-300 p-2 rounded focus:outline-none focus:ring focus:ring-blue-300"
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
          onClick={handleRemove}
        >
          <FaTrash />
          Remove
        </button>
        <button
          className="bg-gray-500 text-white px-4 py-2 rounded flex items-center gap-1 hover:bg-gray-600 transition-colors duration-300"
          onClick={handleClear}
        >
          <FaTrash className="rotate-180" />
          Clear
        </button>
        <button
          className="bg-gray-500 text-white px-4 py-2 rounded flex items-center gap-1 hover:bg-gray-600 transition-colors duration-300"
          onClick={handleFetch}
        >
          <FaSync className="animate-spin" />
          Refresh
        </button>
      </div>

      {/* Size Info */}
      <div className="mb-4 text-sm text-gray-600">
        <strong>Size:</strong> {list.length}
      </div>

      {/* Sorted List Display */}
      <div className="flex flex-wrap items-center justify-center mt-2">
        <AnimatePresence>
          {list.map((node, i) => {
            const isHead = i === 0;
            const isTail = i === list.length - 1;
            return (
              <React.Fragment key={node.id}>
                <motion.div
                  initial={{ scale: 0, opacity: 0 }}
                  animate={{ scale: 1, opacity: 1 }}
                  exit={{
                    scale: 2,
                    rotate: 45,
                    opacity: 0,
                    transition: { duration: 0.3 },
                  }}
                  layout
                  className="flex flex-row items-center gap-2 mx-2 my-4"
                >
                  {isHead && <div className="text-sm text-gray-700">Head</div>}
                  <div className="text-sm text-gray-600">Index: {i}</div>
                  <div className="bg-purple-600 text-white px-4 py-2 rounded shadow min-w-[4rem] text-center text-lg font-bold">
                    {node.value}
                  </div>
                  {isTail && list.length > 1 && (
                    <div className="text-sm text-gray-700">Tail</div>
                  )}
                </motion.div>
                {!isTail && (
                  <div className="flex items-center text-gray-500 text-2xl">
                    <FaArrowRight />
                  </div>
                )}
              </React.Fragment>
            );
          })}
        </AnimatePresence>
      </div>
    </div>
  );
}

export default SortedListSection;
