// src/components/DataNode.js
import React from "react";
import { motion } from "framer-motion"; // optional if you want animations

// Basic node styles, plus optional "vertical" or "horizontal" layout
function DataNode({
  value,
  index,
  label,       // e.g. "Head", "Tail", "Front", "Rear", "Top", "Bottom"
  showIndex,   // boolean
  layout = "horizontal" // or "vertical"
}) {
  return (
    <motion.div
      className="flex flex-col items-center mx-2"
      initial={{ opacity: 0, scale: 0.6 }}
      animate={{ opacity: 1, scale: 1 }}
      transition={{ duration: 0.2 }}
    >
      {/* Label if provided (e.g., Head, Tail) */}
      {label && <div className="ds-label">{label}</div>}

      {/* Index above the node */}
      {showIndex && (
        <div className="ds-index">
          {index}
        </div>
      )}

      {/* The node's value */}
      <div className="bg-blue-600 text-white px-4 py-2 rounded shadow min-w-[3rem] text-center">
        {value}
      </div>
    </motion.div>
  );
}

export default DataNode;
