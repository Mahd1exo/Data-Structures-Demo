// src/components/LinkedListSection.js
import React, { useState, useEffect } from "react";
import {
  fetchLinkedList,
  addFrontLinkedList,
  addEndLinkedList,
  addByIndexLinkedList,
  removeFrontLinkedList,
  removeEndLinkedList,
  removeByIndexLinkedList,
  clearLinkedList,
} from "../services/api";
import { AnimatePresence, motion } from "framer-motion";
import {
  FaPlus,
  FaTrash,
  FaSync,
  FaInfoCircle,
  FaArrowRight,
} from "react-icons/fa";

// Generate a unique ID for local mapping
function genId() {
  return Date.now().toString(36) + Math.random().toString(36).substring(2);
}

function LinkedListSection() {
  const [list, setList] = useState([]);
  const [inputValue, setInputValue] = useState("");
  const [addIndex, setAddIndex] = useState("");
  const [removeIndex, setRemoveIndex] = useState("");
  const [errorMessage, setErrorMessage] = useState("");
  const [showInfoModal, setShowInfoModal] = useState(false);

  // Fetch linked list on mount
  useEffect(() => {
    handleFetch();
    // eslint-disable-next-line
  }, []);

  async function handleFetch() {
    try {
      const data = await fetchLinkedList(); // expects { list: ["val1", "val2", ...] }
      const rawArray = data.list || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setList(mapped);
      setErrorMessage("");
    } catch (err) {
      console.error("Error fetching linked list:", err);
      setErrorMessage("Error fetching linked list.");
    }
  }

  // Clear the entire linked list
  async function handleClear() {
    setErrorMessage("");
    try {
      const data = await clearLinkedList();
      const rawArray = data.list || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setList(mapped);
    } catch (err) {
      console.error("Error clearing linked list:", err);
      setErrorMessage("Error clearing linked list.");
    }
  }

  // Add at front with validation
  async function handleAddFront() {
    const value = inputValue.trim();
    if (!value) {
      setErrorMessage("Please enter a valid value to add.");
      return;
    }
    if (value.length > 16) {
      setErrorMessage("Value must be 16 characters or less.");
      return;
    }
    setErrorMessage("");
    try {
      const data = await addFrontLinkedList(value);
      const rawArray = data.list || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setList(mapped);
      setInputValue("");
    } catch (err) {
      console.error("Error adding to front:", err);
      setErrorMessage("Error adding value to front.");
    }
  }

  // Add at end with validation
  async function handleAddEnd() {
    const value = inputValue.trim();
    if (!value) {
      setErrorMessage("Please enter a valid value to add.");
      return;
    }
    if (value.length > 16) {
      setErrorMessage("Value must be 16 characters or less.");
      return;
    }
    setErrorMessage("");
    try {
      const data = await addEndLinkedList(value);
      const rawArray = data.list || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setList(mapped);
      setInputValue("");
    } catch (err) {
      console.error("Error adding to end:", err);
      setErrorMessage("Error adding value to end.");
    }
  }

  // Add by index: requires both an index and a value
  async function handleAddByIndex() {
    const value = inputValue.trim();
    const idx = addIndex.trim();
    if (!value) {
      setErrorMessage("Please enter a valid value to add.");
      return;
    }
    if (!idx || isNaN(idx)) {
      setErrorMessage("Please enter a valid numeric index for insertion.");
      return;
    }
    if (value.length > 16) {
      setErrorMessage("Value must be 16 characters or less.");
      return;
    }
    setErrorMessage("");
    const indexNum = parseInt(idx, 10);
    try {
      const data = await addByIndexLinkedList(indexNum, value);
      const rawArray = data.list || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setList(mapped);
      setInputValue("");
      setAddIndex("");
    } catch (err) {
      console.error("Error adding by index:", err);
      setErrorMessage("Error adding value by index.");
    }
  }

  // Remove from front
  async function handleRemoveFront() {
    setErrorMessage("");
    try {
      const data = await removeFrontLinkedList();
      const rawArray = data.list || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setList(mapped);
    } catch (err) {
      console.error("Error removing from front:", err);
      setErrorMessage("Error removing value from front.");
    }
  }

  // Remove from end
  async function handleRemoveEnd() {
    setErrorMessage("");
    try {
      const data = await removeEndLinkedList();
      const rawArray = data.list || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setList(mapped);
    } catch (err) {
      console.error("Error removing from end:", err);
      setErrorMessage("Error removing value from end.");
    }
  }

  // Remove by index with validation
  async function handleRemoveByIndex() {
    const idx = removeIndex.trim();
    if (idx === "") {
      setErrorMessage("Please enter a valid index to remove.");
      return;
    }
    if (isNaN(idx)) {
      setErrorMessage("Index must be a number.");
      return;
    }
    const indexNum = parseInt(idx, 10);
    if (indexNum < 0 || indexNum >= list.length) {
      setErrorMessage("Index out of bounds.");
      return;
    }
    setErrorMessage("");
    try {
      const data = await removeByIndexLinkedList(indexNum);
      const rawArray = data.list || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setList(mapped);
      setRemoveIndex("");
    } catch (err) {
      console.error("Error removing by index:", err);
      setErrorMessage("Error removing value by index.");
    }
  }

  function toggleInfoModal() {
    setShowInfoModal((prev) => !prev);
  }

  return (
    <div className="w-full p-6">
      {/* Header: Title and Info Button */}
      <div className="flex justify-between items-center mb-4">
        <h2 className="text-3xl font-bold text-gray-800">Linked List</h2>
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
                About Linked Lists
              </h3>
              <p className="text-gray-700 text-base leading-relaxed mb-4">
                A <strong>linked list</strong> is a linear data structure where each node
                contains a value and a reference to the next node. It allows efficient
                insertions and deletions at either end as well as at a specified position.
              </p>
              <button
                onClick={toggleInfoModal}
                className="ml- bg-indigo-600 hover:bg-indigo-700 text-white px-4 py-2 rounded transition-colors duration-300"
              >
                Close
              </button>

            </motion.div>
          </motion.div>
        )}
      </AnimatePresence>

      {/* Combined Control Row */}
      <div className="mb-4 p-4 border rounded-md shadow-sm bg-white flex flex-col gap-4">
        {/* First Row: Add Controls */}
        <div className="flex w-full items-center gap-4">
          <input
            type="text"
            placeholder="Index for Add (optional)"
            className="w-1/4 border border-gray-300 p-2 rounded focus:outline-none focus:ring focus:ring-blue-300"
            value={addIndex}
            onChange={(e) => setAddIndex(e.target.value)}
          />
          <input
            type="text"
            placeholder="Enter value"
            className="w-1/4 border border-gray-300 p-2 rounded text-lg focus:outline-none focus:ring focus:ring-blue-300"
            value={inputValue}
            onChange={(e) => {
              setInputValue(e.target.value);
              if (e.target.value.trim()) setErrorMessage("");
            }}
          />
          <div className="flex-1 flex gap-2">
            <button
              className="flex-1 flex items-center justify-center gap-1 bg-blue-500 hover:bg-blue-600 text-white rounded text-base font-semibold transition-colors duration-300 px-4 py-3"
              onClick={handleAddFront}
            >
              <FaPlus />
              Add Front
            </button>
            <button
              className="flex-1 flex items-center justify-center gap-1 bg-blue-500 hover:bg-blue-600 text-white rounded text-base font-semibold transition-colors duration-300 px-4 py-3"
              onClick={handleAddEnd}
            >
              <FaPlus />
              Add End
            </button>
            <button
              className="flex-1 flex items-center justify-center gap-1 bg-blue-500 hover:bg-blue-600 text-white rounded text-base font-semibold transition-colors duration-300 px-4 py-3"
              onClick={handleAddByIndex}
            >
              <FaPlus />
              Add by Index
            </button>
          </div>
        </div>
        {/* Second Row: Remove Controls */}
        <div className="flex w-full items-center gap-4">
          <input
            type="text"
            placeholder="Index to remove"
            className="w-1/4 border border-gray-300 p-2 rounded focus:outline-none focus:ring focus:ring-red-300"
            value={removeIndex}
            onChange={(e) => setRemoveIndex(e.target.value)}
          />
          <div className="flex-1 flex gap-2">
            <button
              className="flex-1 flex items-center justify-center gap-1 bg-red-500 hover:bg-red-600 text-white rounded text-base font-semibold transition-colors duration-300 px-4 py-3"
              onClick={handleRemoveFront}
            >
              <FaTrash />
              Remove Front
            </button>
            <button
              className="flex-1 flex items-center justify-center gap-1 bg-red-500 hover:bg-red-600 text-white rounded text-base font-semibold transition-colors duration-300 px-4 py-3"
              onClick={handleRemoveEnd}
            >
              <FaTrash />
              Remove End
            </button>
            <button
              className="flex-1 flex items-center justify-center gap-1 bg-red-500 hover:bg-red-600 text-white rounded text-base font-semibold transition-colors duration-300 px-4 py-3"
              onClick={handleRemoveByIndex}
            >
              <FaTrash />
              Remove by Index
            </button>
          </div>
          {/* Wrap Refresh and Clear in a container that's half the width */}
          <div className="w-1/4 flex gap-2">
            <button
              className="flex-1 flex items-center justify-center gap-1 bg-gray-500 hover:bg-gray-600 text-white rounded text-base font-semibold transition-colors duration-300 px-2 py-3"
              onClick={handleFetch}
            >
              <FaSync className="animate-spin" />
              Refresh
            </button>
            <button
              className="flex-1 flex items-center justify-center gap-1 bg-gray-500 hover:bg-gray-600 text-white rounded text-base font-semibold transition-colors duration-300 px-2 py-3"
              onClick={handleClear}
            >
              <FaTrash className="rotate-180" />
              Clear
            </button>
          </div>
        </div>
      </div>

      {/* Size Info */}
      <div className="mb-4 text-sm text-gray-600">
        <strong>Size:</strong> {list.length}
      </div>

      {/* Linked List Display */}
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
                  {isHead && (
                    <div className="text-sm text-gray-700">Head</div>
                  )}
                  <div className="text-sm text-gray-600">Index: {i}</div>
                  <div className="bg-blue-600 text-white px-4 py-2 rounded shadow min-w-[4rem] text-center text-lg font-bold">
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

export default LinkedListSection;
