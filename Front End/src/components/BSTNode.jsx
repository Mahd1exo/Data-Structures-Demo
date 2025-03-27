// src/components/BSTNode.js
import React from "react";
import { FaArrowDown } from "react-icons/fa";
import { motion } from "framer-motion";

function NullNode() {
  return (
    <motion.div
      className="flex items-center justify-center w-16 h-16 rounded-full border-2 border-dashed border-gray-400 bg-gray-100 text-gray-500 font-bold shadow-sm"
      initial={{ scale: 0.8, opacity: 0.5 }}
      animate={{ scale: 1, opacity: 1 }}
      transition={{ duration: 0.3 }}
    >
      null
    </motion.div>
  );
}

function BSTNode({ node, level = 0 }) {
  if (!node) return null;

  const hasLeft = node.left !== null;
  const hasRight = node.right !== null;

  return (
    <motion.div
      className="bst-node flex flex-col items-center"
      initial={{ opacity: 0, y: -10 }}
      animate={{ opacity: 1, y: 0 }}
      transition={{ duration: 0.3 }}
    >
      {/* Node Header: showing level */}
      <div className="node-header text-xs text-gray-500 mb-1">
        Level {level}
      </div>
      {/* Node displayed as a circle */}
      <motion.div
        className="node-content bg-blue-600 text-white w-16 h-16 rounded-full border-2 border-black flex items-center justify-center font-bold shadow-sm"
        initial={{ scale: 0.8, opacity: 0 }}
        animate={{ scale: 1, opacity: 1 }}
        transition={{ duration: 0.3 }}
      >
        {node.value}
      </motion.div>
      {/* Render children if at least one exists */}
      {(hasLeft || hasRight) && (
        <div className="children flex justify-center gap-4 mt-4">
          {/* Left Child */}
          <div className="flex flex-col items-center">
            <FaArrowDown className="text-gray-500" />
            {hasLeft ? (
              <BSTNode node={node.left} level={level + 1} />
            ) : (
              <NullNode />
            )}
          </div>
          {/* Right Child */}
          <div className="flex flex-col items-center">
            <FaArrowDown className="text-gray-500" />
            {hasRight ? (
              <BSTNode node={node.right} level={level + 1} />
            ) : (
              <NullNode />
            )}
          </div>
        </div>
      )}
    </motion.div>
  );
}

export default BSTNode;
