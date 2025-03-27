// src/components/MinHeapNode.js
import React from "react";
import { FaArrowDown } from "react-icons/fa";
import { motion } from "framer-motion";

function MinHeapNode({ heap, index, level = 0 }) {
  if (index >= heap.length || heap[index] === null) {
    return (
      <motion.div
        className="flex items-center justify-center w-12 h-12 rounded-full border-2 border-dashed border-gray-400 bg-gray-100 text-gray-500 font-bold shadow-sm"
        initial={{ scale: 0.8, opacity: 0.5 }}
        animate={{ scale: 1, opacity: 1 }}
        transition={{ duration: 0.3 }}
      >
        null
      </motion.div>
    );
  }
  return (
    <motion.div
      className="flex flex-col items-center relative"
      initial={{ opacity: 0, y: -10 }}
      animate={{ opacity: 1, y: 0 }}
      transition={{ duration: 0.3 }}
    >
      <div className="text-xs text-gray-500 mb-1">Level {level}</div>
      <motion.div
        className="w-12 h-12 rounded-full border-2 border-black bg-purple-500 text-white font-bold flex items-center justify-center shadow-sm"
        initial={{ scale: 0.8, opacity: 0 }}
        animate={{ scale: 1, opacity: 1 }}
        transition={{ duration: 0.3 }}
      >
        {heap[index]}
      </motion.div>
      {((2 * index + 1) < heap.length || (2 * index + 2) < heap.length) && (
        <div className="mt-4 flex justify-center gap-4 relative">
          <div className="absolute top-0 left-0 right-0 border-t-2 border-gray-300"></div>
          <div className="flex flex-col items-center">
            <FaArrowDown className="text-gray-500" />
            <MinHeapNode heap={heap} index={2 * index + 1} level={level + 1} />
          </div>
          <div className="flex flex-col items-center">
            <FaArrowDown className="text-gray-500" />
            <MinHeapNode heap={heap} index={2 * index + 2} level={level + 1} />
          </div>
        </div>
      )}
    </motion.div>
  );
}

export default MinHeapNode;
