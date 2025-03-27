// src/components/CircularListSection.js
import React, { useState, useEffect } from "react";
import {
  fetchCircularList,
  circularListInsert,
  circularListInsertByIndex,
  circularListRemove,
  circularListRemoveFront,
  circularListRemoveEnd,
  circularListRemoveByIndex,
  circularListClear,
  circularListInsertFront,
} from "../services/api";
import { motion, AnimatePresence } from "framer-motion";
import { FaPlus, FaTrash, FaSync, FaInfoCircle } from "react-icons/fa";
import CircularNode from "./CircularNode";

// Generate a unique ID for local mapping
function genId() {
  return Date.now().toString(36) + Math.random().toString(36).substring(2);
}

function CircularListSection() {
  const [list, setList] = useState([]);
  const [inputValue, setInputValue] = useState("");
  const [addIndex, setAddIndex] = useState("");
  const [removeIndex, setRemoveIndex] = useState("");
  const [errorMessage, setErrorMessage] = useState("");
  const [showInfoModal, setShowInfoModal] = useState(false);

  // Layout parameters
  const containerSize = 450;
  const radius = 150;
  const nodeSize = 60;
  const halfNode = nodeSize / 2;
  const centerX = containerSize / 2;
  const centerY = containerSize / 2;
  const arrowExtraOffset = 10;

  useEffect(() => {
    handleFetch();
  }, []);

  async function handleFetch() {
    try {
      const data = await fetchCircularList();
      const rawArray = data.circularList || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setList(mapped);
      setErrorMessage("");
    } catch (err) {
      console.error("Error fetching circular list:", err);
      setErrorMessage("Error fetching circular list.");
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
      const data = await circularListInsert(value); // Default: add at end
      const rawArray = data.circularList || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setList(mapped);
      setInputValue("");
    } catch (err) {
      console.error("Error inserting value:", err);
      setErrorMessage("Error inserting value.");
    }
  }
  async function handleInsertFront() {
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
      const data = await circularListInsertFront(value);
      const rawArray = data.circularList || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setList(mapped);
      setInputValue("");
    } catch (err) {
      console.error("Error inserting value:", err);
      setErrorMessage("Error inserting value.");
    }
  }

  async function handleInsertByIndex() {
    const value = inputValue.trim();
    const idx = addIndex.trim();
    if (!value) {
      setErrorMessage("Please enter a valid value to insert.");
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
      const data = await circularListInsertByIndex(indexNum, value);
      const rawArray = data.circularList || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setList(mapped);
      setInputValue("");
      setAddIndex("");
    } catch (err) {
      console.error("Error inserting by index:", err);
      setErrorMessage("Error inserting value by index.");
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
      const data = await circularListRemove(value);
      const rawArray = data.circularList || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setList(mapped);
      setInputValue("");
    } catch (err) {
      console.error("Error removing value:", err);
      setErrorMessage("Error removing value.");
    }
  }

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
      const data = await circularListRemoveByIndex(indexNum);
      const rawArray = data.circularList || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setList(mapped);
      setRemoveIndex("");
    } catch (err) {
      console.error("Error removing by index:", err);
      setErrorMessage("Error removing value by index.");
    }
  }

  async function handleRemoveFront() {
    setErrorMessage("");
    try {
      const data = await circularListRemoveFront();
      const rawArray = data.circularList || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setList(mapped);
    } catch (err) {
      console.error("Error removing front:", err);
      setErrorMessage("Error removing front value.");
    }
  }

  async function handleRemoveEnd() {
    setErrorMessage("");
    try {
      const data = await circularListRemoveEnd();
      const rawArray = data.circularList || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setList(mapped);
    } catch (err) {
      console.error("Error removing end:", err);
      setErrorMessage("Error removing end value.");
    }
  }

  async function handleClear() {
    setErrorMessage("");
    try {
      const data = await circularListClear();
      const rawArray = data.circularList || [];
      const mapped = rawArray.map((val) => ({ id: genId(), value: val }));
      setList(mapped);
    } catch (err) {
      console.error("Error clearing list:", err);
      setErrorMessage("Error clearing the list.");
    }
  }

  function toggleInfoModal() {
    setShowInfoModal((prev) => !prev);
  }

  // Compute positions and arrow lines only if list is not empty
  let positions = [];
  let arrowLines = [];
  if (list.length > 0) {
    positions = list.map((_, i) => {
      const angle = (2 * Math.PI * i) / list.length;
      return {
        x: centerX + radius * Math.cos(angle) - halfNode,
        y: centerY + radius * Math.sin(angle) - halfNode,
      };
    });
    if (list.length > 1) {
      for (let i = 0; i < list.length; i++) {
        const nextIndex = (i + 1) % list.length;
        const startPos = {
          x: centerX + radius * Math.cos((2 * Math.PI * i) / list.length),
          y: centerY + radius * Math.sin((2 * Math.PI * i) / list.length),
        };
        const endPos = {
          x: centerX + radius * Math.cos((2 * Math.PI * nextIndex) / list.length),
          y: centerY + radius * Math.sin((2 * Math.PI * nextIndex) / list.length),
        };
        const dx = endPos.x - startPos.x;
        const dy = endPos.y - startPos.y;
        const dist = Math.sqrt(dx * dx + dy * dy);
        const offsetX = (dx / dist) * halfNode;
        const offsetY = (dy / dist) * halfNode;
        const extraX = (dx / dist) * arrowExtraOffset;
        const extraY = (dy / dist) * arrowExtraOffset;
        arrowLines.push({
          start: { x: startPos.x + offsetX, y: startPos.y + offsetY },
          end: { x: endPos.x - offsetX + extraX, y: endPos.y - offsetY + extraY },
        });
      }
    }
  }

  return (
    <div className="w-full p-6">
      {/* Header */}
      <div className="flex justify-between items-center mb-4">
        <h2 className="text-3xl font-bold text-gray-800">Circular List</h2>
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
                About Circular Lists
              </h3>
              <p className="text-gray-700 text-base leading-relaxed mb-4">
                A <strong>circular list</strong> is a linked list where the tail
                points back to the head, forming a cycle. You can add elements at
                the front, end, or a specific index, and remove them similarly.
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
        {/* Row 1: Add Controls */}
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
              className="flex-1 flex items-center justify-center gap-1 bg-blue-500 hover:bg-blue-600 text-white rounded inline-flex text-base font-semibold transition-colors duration-300 px-4 py-3"
              onClick={handleInsertFront}
            >
              <FaPlus />
              Add Front
            </button>
            <button
              className="flex-1 flex items-center justify-center gap-1 bg-blue-500 hover:bg-blue-600 text-white rounded inline-flex text-base font-semibold transition-colors duration-300 px-4 py-3"
              onClick={handleInsertByIndex}
            >
              <FaPlus />
              Add by Index
            </button>
            <button
              className="flex-1 flex items-center justify-center gap-1 bg-blue-500 hover:bg-blue-600 text-white rounded inline-flex text-base font-semibold transition-colors duration-300 px-4 py-3"
              onClick={handleInsert} 
            >
              <FaPlus />
              Add End
            </button>
          </div>
        </div>
        {/* Row 2: Remove Controls with Refresh & Clear Buttons */}
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
              className="flex-1 flex items-center justify-center gap-1 bg-red-500 hover:bg-red-600 text-white rounded inline-flex text-base font-semibold transition-colors duration-300 px-4 py-3"
              onClick={handleRemoveFront}
            >
              <FaTrash />
              Remove Front
            </button>
            <button
              className="flex-1 flex items-center justify-center gap-1 bg-red-500 hover:bg-red-600 text-white rounded inline-flex text-base font-semibold transition-colors duration-300 px-4 py-3"
              onClick={handleRemoveEnd}
            >
              <FaTrash />
              Remove End
            </button>
            <button
              className="flex-1 flex items-center justify-center gap-1 bg-red-500 hover:bg-red-600 text-white rounded inline-flex text-base font-semibold transition-colors duration-300 px-4 py-3"
              onClick={handleRemoveByIndex}
            >
              <FaTrash />
              Remove by Index
            </button>
          </div>
          <div className="w-1/4 flex gap-2">
            <button
              className="flex-1 flex items-center justify-center gap-1 bg-gray-500 hover:bg-gray-600 text-white rounded inline-flex text-base font-semibold transition-colors duration-300 px-4 py-3"
              onClick={handleFetch}
            >
              <FaSync className="animate-spin" />
              Refresh
            </button>
            <button
              className="flex-1 flex items-center justify-center gap-1 bg-gray-500 hover:bg-gray-600 text-white rounded inline-flex text-base font-semibold transition-colors duration-300 px-4 py-3"
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

{/* Display Area */}
{list.length === 0 ? (
  <div
    className="relative mx-auto flex flex-col items-center justify-center text-center text-gray-600"
    style={{
      width: containerSize,
      height: containerSize,
      border: "1px dashed #ccc",
      borderRadius: "50%",
    }}
  >
    <div>
      <strong>Size: 0</strong>
    </div>
    <div>Empty</div>
  </div>
) : (
  <div
    className="relative mx-auto"
    style={{
      width: containerSize,
      height: containerSize,
      border: "1px dashed #ccc",
      borderRadius: "50%",
    }}
  >
    <AnimatePresence>
      {list.map((node, i) => {
        // container center
        const centerX = containerSize / 2; // 200
        const centerY = containerSize / 2; // 200

        // we want the *outer edge* of the node to be on the dashed circle
        const nodeSize = 65;
        const halfNode = nodeSize / 2;

        // dashed circle radius is 200
        // place node center at (200 - halfNode) from center
        const circleRadius = 200 - halfNode; // ~167.5

        // angle for each node
        const angle = (2 * Math.PI * i) / list.length;

        const x = centerX + circleRadius * Math.cos(angle) - halfNode;
        const y = centerY + circleRadius * Math.sin(angle) - halfNode;

        let nextX = null;
        let nextY = null;
        if (list.length > 1) {
          const nextIndex = (i + 1) % list.length;
          const angle2 = (2 * Math.PI * nextIndex) / list.length;
          nextX = centerX + circleRadius * Math.cos(angle2) - halfNode;
          nextY = centerY + circleRadius * Math.sin(angle2) - halfNode;
        }

        return (
          <CircularNode
            key={node.id}
            node={node}
            index={i}
            total={list.length}
            x={x}
            y={y}
            nodeSize={nodeSize}
            nextX={nextX}
            nextY={nextY}
            arrowExtraOffset={10} // arrow extension
            centerX={centerX}
            centerY={centerY}
          />
        );
      })}
    </AnimatePresence>

    {/* For the big arrows that connect each node in the circle, if needed */}
    {list.length > 1 && (
      <svg
        className="absolute"
        style={{ top: 0, left: 0, width: containerSize, height: containerSize }}
      >
        <defs>
          <marker
            id="arrowhead"
            markerWidth="10"
            markerHeight="7"
            refX="0"
            refY="3.5"
            orient="auto"
          >
            <polygon points="0 0, 10 3.5, 0 7" fill="#333" />
          </marker>
        </defs>
        {list.map((_, i) => {
          const centerX = containerSize / 2;
          const centerY = containerSize / 2;
          const nodeSize = 65;
          const halfNode = nodeSize / 2;
          const margin = 10;
          const circleRadius = 200 - halfNode;
          const angle1 = (2 * Math.PI * i) / list.length;
          const angle2 = (2 * Math.PI * ((i + 1) % list.length)) / list.length;

          const startPos = {
            x: centerX + circleRadius * Math.cos(angle1),
            y: centerY + circleRadius * Math.sin(angle1),
          };
          const endPos = {
            x: centerX + circleRadius * Math.cos(angle2),
            y: centerY + circleRadius * Math.sin(angle2),
          };
          const dx = endPos.x - startPos.x;
          const dy = endPos.y - startPos.y;
          const dist = Math.sqrt(dx * dx + dy * dy) || 1;

          const startX = startPos.x + (dx / dist) * (halfNode + margin);
          const startY = startPos.y + (dy / dist) * (halfNode + margin);
          const arrowExtraOffset = -15;
          const endX =
            endPos.x -
            (dx / dist) * (halfNode + margin) +
            (dx / dist) * arrowExtraOffset;
          const endY =
            endPos.y -
            (dy / dist) * (halfNode + margin) +
            (dy / dist) * arrowExtraOffset;

          return (
            <line
              key={i}
              x1={startX}
              y1={startY}
              x2={endX}
              y2={endY}
              stroke="#333"
              strokeWidth="2"
              markerEnd="url(#arrowhead)"
            />
          );
        })}
      </svg>
    )}
  </div>
)}




    </div>
  );
}

export default CircularListSection;
