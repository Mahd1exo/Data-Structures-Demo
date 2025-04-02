// src/components/StackSection.js
import React, { useState, useEffect } from "react";
import { fetchStack, pushStack, popStack, clearStack } from "../services/api";
import { AnimatePresence, motion } from "framer-motion";
import Tilt from "react-parallax-tilt";
import {
  FaPlus,
  FaMinus,
  FaSync,
  FaInfoCircle,
  FaChevronDown,
  FaChevronUp,
  FaSort,
  FaTrash,
} from "react-icons/fa";

// Generate a unique ID for each stack item
function genId() {
  return Date.now().toString(36) + Math.random().toString(36).substring(2);
}

function StackSection() {
  const [stackItems, setStackItems] = useState([]);
  const [inputValue, setInputValue] = useState("");
  const [isExpanded, setIsExpanded] = useState(false);
  const [isReversed, setIsReversed] = useState(false);
  const [errorMessage, setErrorMessage] = useState("");
  const [showInfoModal, setShowInfoModal] = useState(false);

  useEffect(() => {
    handleFetch();
  }, []);

  async function handleFetch() {
    try {
      const data = await fetchStack();
      const rawArray = data.stack || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setStackItems(mapped);
    } catch (err) {
      console.error("Error fetching stack:", err);
    }
  }

  async function handlePush() {
    if (!inputValue.trim()) {
      setErrorMessage("Please enter a valid value.");
      return;
    }
    setErrorMessage("");
    try {
      const data = await pushStack(inputValue.trim());
      const rawArray = data.stack || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setStackItems(mapped);
      setInputValue("");
    } catch (err) {
      console.error("Error pushing stack:", err);
    }
  }

  async function handlePop() {
    try {
      const data = await popStack();
      const rawArray = data.stack || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setStackItems(mapped);
    } catch (err) {
      console.error("Error popping stack:", err);
    }
  }

  async function handleClear() {
    try {
      const data = await clearStack();
      const rawArray = data.stack || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setStackItems(mapped);
    } catch (err) {
      console.error("Error clearing stack:", err);
      setErrorMessage("Error clearing stack.");
    }
  }

  function toggleExpand() {
    setIsExpanded((prev) => !prev);
  }
  function toggleReverse() {
    setIsReversed((prev) => !prev);
  }
  function toggleInfoModal() {
    setShowInfoModal((prev) => !prev);
  }

  // Prepare items for display based on reverse toggle
  const displayedItems = isReversed ? [...stackItems].reverse() : stackItems;

  function renderStack() {
    if (!displayedItems.length) {
      return <p className="text-gray-500 text-center">Stack is empty</p>;
    }

    if (!isExpanded) {
      // Overlapping deck view
      return (
        <div
          className="relative w-full max-w-md mx-auto overflow-visible"
          style={{ perspective: "1200px", height: "350px" }}
        >
          <AnimatePresence>
            {displayedItems.map((item, i) => {
              const isTop = i === 0;
              const isBottom = i === displayedItems.length - 1;
              const offsetStep = 10;
              const verticalOffset = i * offsetStep;
              const rotationAngle = i * 1;
              const zIndex = displayedItems.length - i;

              return (
                <Tilt
                  key={item.id}
                  glareEnable={true}
                  glareMaxOpacity={0.15}
                  scale={1.02}
                  transitionSpeed={250}
                  tiltMaxAngleX={5}
                  tiltMaxAngleY={5}
                  className="absolute w-full"
                  style={{ top: verticalOffset, zIndex }}
                >
                  <motion.div
                    className="mx-auto"
                    initial={{ scale: 0.8, opacity: 0, y: -15 }}
                    animate={{ scale: 1, opacity: 1, y: 0, rotate: rotationAngle }}
                    exit={{ scale: 0.8, opacity: 0, y: 15, rotate: rotationAngle }}
                    transition={{ duration: 0.4 }}
                  >
                    <div className="p-6 rounded-xl shadow-xl border border-gray-200 bg-gradient-to-r from-blue-50 to-blue-100">
                      <div className="flex justify-end mb-2">
                        {isTop && (
                          <span className="bg-green-500 text-white text-xs font-bold px-2 py-1 rounded-full">
                            TOP
                          </span>
                        )}
                        {isBottom && displayedItems.length > 1 && (
                          <span className="bg-red-500 text-white text-xs font-bold px-2 py-1 rounded-full">
                            BOTTOM
                          </span>
                        )}
                      </div>
                      <div className="text-center">
                        <div className="text-2xl font-bold text-gray-800">
                          {item.value}
                        </div>
                      </div>
                    </div>
                  </motion.div>
                </Tilt>
              );
            })}
          </AnimatePresence>
        </div>
      );
    } else {
      // Expanded list view
      return (
        <div className="flex flex-col gap-4 w-full max-w-md mx-auto my-8">
          <AnimatePresence>
            {displayedItems.map((item) => (
              <motion.div
                key={item.id}
                className="w-full"
                initial={{ opacity: 0, y: -10 }}
                animate={{ opacity: 1, y: 0 }}
                exit={{ opacity: 0, y: 10 }}
                transition={{ duration: 0.3 }}
              >
                <div className="p-6 rounded-xl shadow-md border border-gray-200 bg-white">
                  <div className="flex justify-end mb-2">
                    {displayedItems[0].id === item.id && (
                      <span className="bg-green-500 text-white text-xs font-bold px-2 py-1 rounded-full">
                        TOP
                      </span>
                    )}
                    {displayedItems[displayedItems.length - 1].id === item.id &&
                      displayedItems.length > 1 && (
                        <span className="bg-red-500 text-white text-xs font-bold px-2 py-1 rounded-full">
                          BOTTOM
                        </span>
                      )}
                  </div>
                  <div className="text-center">
                    <div className="text-2xl font-bold text-gray-800">
                      {item.value}
                    </div>
                  </div>
                </div>
              </motion.div>
            ))}
          </AnimatePresence>
        </div>
      );
    }
  }

  return (
    <div className="w-full p-6">
      {/* Header: Title and Info Button */}
      <div className="flex justify-between items-center mb-4">
        <h2 className="text-3xl font-bold text-gray-800 text-left">
          Stack Manager
        </h2>
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
                About Stacks
              </h3>
              <p className="text-gray-700 text-base leading-relaxed mb-4">
                A <strong>stack</strong> is a linear data structure that follows the Last-In-First-Out (LIFO) principle.
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

      {/* Backend Order Note */}
      <div className="mb-4 text-left text-gray-600">
        <strong>Backend Order:</strong> [0 = TOP, …, last = BOTTOM]
      </div>

      {/* Action Panel */}
      <div className="mb-4 p-4 border rounded-md shadow-sm bg-white flex items-center gap-4">
        <input
          type="text"
          placeholder="Enter value"
          className="border border-gray-300 p-2 rounded flex-1 focus:outline-none focus:ring focus:ring-blue-300"
          value={inputValue}
          onChange={(e) => {
            setInputValue(e.target.value);
            if (e.target.value.trim()) setErrorMessage("");
          }}
        />
        <button
          className="bg-blue-500 text-white px-4 py-2 rounded flex items-center gap-1 hover:bg-blue-600 transition-colors duration-300"
          onClick={handlePush}
        >
          <FaPlus />
          Push
        </button>
        <button
          className="bg-red-500 hover:bg-red-600 text-white px-4 py-2 rounded flex items-center gap-1 font-semibold transition-colors duration-300"
          onClick={handlePop}
        >
          <FaMinus />
          Pop
        </button>
        <button
          className="bg-gray-500 text-white px-4 py-2 rounded flex items-center gap-1 hover:bg-gray-600 transition-colors duration-300"
          onClick={handleClear}
        >
          <FaTrash className="rotate-180" />
          Clear
        </button>
        <button
          className="bg-gray-500 hover:bg-gray-600 text-white px-4 py-2 rounded flex items-center gap-1 font-semibold transition-colors duration-300"
          onClick={handleFetch}
        >
          <FaSync className="animate-spin" />
          Refresh
        </button>
        <button
          className="bg-green-800 hover:bg-green-700 text-white px-4 py-2 rounded flex items-center gap-1 font-semibold transition-colors duration-300"
          onClick={toggleExpand}
        >
          {isExpanded ? <FaChevronUp /> : <FaChevronDown />}
          {isExpanded ? "Collapse" : "Expand"}
        </button>
        <button
          className="bg-purple-600 hover:bg-purple-700 text-white px-4 py-2 rounded flex items-center gap-1 font-semibold transition-colors duration-300"
          onClick={toggleReverse}
        >
          <FaSort />
          {isReversed ? "Normal" : "Reverse"}
        </button>
      </div>

      {/* Stack Size Info */}
      <div className="mb-4 text-left text-sm text-gray-600">
        <strong>Stack size:</strong> {stackItems.length}
      </div>

      {/* Stack Display */}
      {renderStack()}
    </div>
  );
}

export default StackSection;
