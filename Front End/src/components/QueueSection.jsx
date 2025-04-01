// src/components/QueueSection.js
import React, { useState, useEffect } from "react";
import { 
  fetchQueue, 
  enqueue as enqueueApi, 
  dequeue as dequeueApi,
  clearQueue  // import clearQueue from API
} from "../services/api";
import { AnimatePresence, motion } from "framer-motion";
import { FaArrowRight, FaPlus, FaMinus, FaSync, FaInfoCircle, FaTrash } from "react-icons/fa";

function genId() {
  return Date.now().toString(36) + Math.random().toString(36).substring(2);
}

function QueueSection() {
  const [items, setItems] = useState([]);
  const [inputValue, setInputValue] = useState("");
  const [errorMessage, setErrorMessage] = useState("");
  const [showInfoModal, setShowInfoModal] = useState(false);

  useEffect(() => {
    handleFetch();
  }, []);

  async function handleFetch() {
    try {
      const data = await fetchQueue(); // Expects { queue: [...] }
      const rawArray = data.queue || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setItems(mapped);
      setErrorMessage("");
    } catch (err) {
      console.error("Error fetching queue:", err);
      setErrorMessage("Error fetching queue.");
    }
  }

  async function handleEnqueue() {
    const value = inputValue.trim();
    if (!value) {
      setErrorMessage("Please enter a valid value.");
      return;
    }
    if (value.length > 16) {
      setErrorMessage("Value must be 16 characters or less.");
      return;
    }
    setErrorMessage("");
    try {
      const data = await enqueueApi(value);
      const rawArray = data.queue || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setItems(mapped);
      setInputValue("");
    } catch (err) {
      console.error("Error enqueueing:", err);
      setErrorMessage("Error enqueueing value.");
    }
  }

  async function handleDequeue() {
    try {
      const data = await dequeueApi();
      const rawArray = data.queue || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setItems(mapped);
    } catch (err) {
      console.error("Error dequeueing:", err);
      setErrorMessage("Error dequeueing value.");
    }
  }

  async function handleClear() {
    try {
      await clearQueue();
      setInputValue("");
      handleFetch();
    } catch (err) {
      console.error("Error clearing queue:", err);
      setErrorMessage("Error clearing queue.");
    }
  }

  function toggleInfoModal() {
    setShowInfoModal((prev) => !prev);
  }

  // Reverse items to display: Left end will be the last node, Right end the first node
  const reversedItems = items.slice().reverse();

  return (
    <div className="w-full p-6">
      {/* Header: Left-Aligned Title, Right-Aligned Info Button */}
      <div className="flex justify-between items-center mb-4">
        <h2 className="text-3xl font-bold text-gray-800 text-left">Queue (Right → Left)</h2>
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
              <h3 className="text-2xl font-bold mb-4 text-gray-800">About Queues</h3>
              <p className="text-gray-700 text-base leading-relaxed mb-4">
                A <strong>queue</strong> is a linear data structure that follows the First In, First Out (FIFO) principle.
                Items are added at the rear and removed from the front, which is perfect for scenarios like task scheduling or buffering.
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

      {/* Action Panel: All controls in a single row */}
      <div className="mb-4 p-4 border rounded-md shadow-sm bg-white flex flex-wrap items-center gap-2 justify-center">
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
          onClick={handleEnqueue}
        >
          <FaPlus />
          Enqueue
        </button>
        <button
          className="bg-red-500 text-white px-4 py-2 rounded flex items-center gap-1 hover:bg-red-600 transition-colors duration-300"
          onClick={handleDequeue}
        >
          <FaMinus />
          Dequeue
        </button>
        <button
          className="bg-gray-500 text-white px-4 py-2 rounded flex items-center gap-1 hover:bg-gray-600 transition-colors duration-300"
          onClick={handleFetch}
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

      {/* Size Info */}
      <div className="mb-4 text-sm text-gray-600">
        <strong>Size:</strong> {items.length}
      </div>

      {/* Queue Display */}
      <div className="flex flex-wrap items-center justify-center mt-2">
        <AnimatePresence>
          {reversedItems.map((item, i) => {
            const isLeftmost = i === 0;
            const isRightmost = i === reversedItems.length - 1;
            return (
              <React.Fragment key={item.id}>
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
                  className="flex items-center gap-1 mx-2 my-4"
                >
                  {isLeftmost && <div className="text-sm text-gray-700">Left</div>}
                  <div className="bg-blue-600 text-white px-4 py-2 rounded shadow text-center text-lg font-bold">
                    {item.value}
                  </div>
                  {isRightmost && reversedItems.length > 1 && (
                    <div className="text-sm text-gray-700">Right</div>
                  )}
                </motion.div>
                {!isRightmost && (
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

export default QueueSection;
