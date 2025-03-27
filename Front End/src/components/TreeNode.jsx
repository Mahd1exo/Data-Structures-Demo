// src/components/TreeNode.js
import React from "react";
import Tilt from "react-parallax-tilt";
import { motion } from "framer-motion";

function TreeNode({ node, index = 0, level = 0, parentId = "" }) {
  if (!node) return null;

  return (
    <motion.div
      className="flex flex-col items-center relative overflow-visible"
      initial={{ opacity: 0, y: -10 }}
      animate={{ opacity: 1, y: 0 }}
      transition={{ duration: 0.3 }}
    >
      {/* Node Header: Level, Index, ID, Parent */}
      <div className="mb-2 text-xs text-gray-600 text-center">
        Level {level} — [Index: {index}] — (ID: {node.id})
        {parentId ? ` — Parent: ${parentId}` : " — Parent: None"}
      </div>

      {/* Node as a green circle with black border */}
      <Tilt
        glareEnable={true}
        glareMaxOpacity={0.2}
        scale={1.02}
        transitionSpeed={250}
        tiltMaxAngleX={5}
        tiltMaxAngleY={5}
        className="w-auto"
      >
        <motion.div
          className="flex items-center justify-center
                     w-16 h-16
                     rounded-full border-2 border-black
                     bg-green-500 text-black font-bold
                     shadow-sm"
          initial={{ scale: 0.8, opacity: 0 }}
          animate={{ scale: 1, opacity: 1 }}
          transition={{ duration: 0.3 }}
        >
          {node.data}
        </motion.div>
      </Tilt>

      {/* If node has children, show them below with vertical/horizontal connectors */}
      {node.children && node.children.length > 0 && (
        <div className="mt-4 relative flex flex-col items-center">
          {/* Short vertical connector from parent to the row of children */}
          <div className="absolute top-0 left-1/2 transform -translate-x-1/2 border-l-2 border-gray-300 h-4"></div>

          {/* Children row */}
          <div className="mt-4 flex justify-center gap-4 relative">
            {/* Horizontal connector across all children */}
            <div className="absolute top-0 left-0 right-0 border-t-2 border-gray-300"></div>
            {node.children.map((child, idx) => (
              <TreeNode
                key={idx}
                node={child}
                index={idx}
                level={level + 1}
                parentId={node.id.toString()}
              />
            ))}
          </div>
        </div>
      )}
    </motion.div>
  );
}

export default TreeNode;
