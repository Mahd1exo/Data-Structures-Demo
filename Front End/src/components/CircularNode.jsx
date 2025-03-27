// src/components/CircularNode.js
import React from "react";
import { motion } from "framer-motion";

function CircularNode({
  node,
  index,
  total,
  x,
  y,
  nodeSize,
  nextX,
  nextY,
  arrowExtraOffset,
  centerX,
  centerY,
}) {
  const halfNode = nodeSize / 2;
  const nodeCenter = { x: x + halfNode, y: y + halfNode };

  // 1) Draw arrow from this node to the next node (if nextX/nextY exist).
  let arrowLine = null;
  if (nextX !== null && nextY !== null && total > 1) {
    const nextCenter = { x: nextX + halfNode, y: nextY + halfNode };
    const dx = nextCenter.x - nodeCenter.x;
    const dy = nextCenter.y - nodeCenter.y;
    const dist = Math.sqrt(dx * dx + dy * dy) || 1;

    // Extra margin so the arrows start/end well outside each node’s boundary.
    const margin = 10;
    const startX = nodeCenter.x + (dx / dist) * (halfNode + margin);
    const startY = nodeCenter.y + (dy / dist) * (halfNode + margin);
    const endX =
      nextCenter.x -
      (dx / dist) * (halfNode + margin) +
      (dx / dist) * arrowExtraOffset;
    const endY =
      nextCenter.y -
      (dy / dist) * (halfNode + margin) +
      (dy / dist) * arrowExtraOffset;

    arrowLine = (
      <svg
        className="absolute pointer-events-none"
        style={{
          position: "absolute",
          left: 0,
          top: 0,
          width: "100%",
          height: "100%",
        }}
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
        <line
          x1={startX}
          y1={startY}
          x2={endX}
          y2={endY}
          stroke="#333"
          strokeWidth="2"
          markerEnd="url(#arrowhead)"
        />
      </svg>
    );
  }

  // 2) Compute external label for the index (with “(Head)” / “(Tail)”).
  const vecX = nodeCenter.x - centerX;
  const vecY = nodeCenter.y - centerY;
  const len = Math.sqrt(vecX * vecX + vecY * vecY) || 1;
  const nx = vecX / len;
  const ny = vecY / len;

  // Place the label a bit beyond the node’s outer boundary.
  const labelOffset = halfNode + 20;
  let labelText = `${index}`;
  if (index === 0) {
    labelText += " (Head)";
  } else if (index === total - 1) {
    labelText += " (Tail)";
  }
  const labelX = nodeCenter.x + nx * labelOffset;
  const labelY = nodeCenter.y + ny * labelOffset;

  return (
    <div
      style={{
        position: "absolute",
        left: x,
        top: y,
        width: nodeSize,
        height: nodeSize,
      }}
    >
      {/* Arrow (if applicable) */}
      {arrowLine}

      {/* 3) The node circle with the node’s value inside */}
      <motion.div
        initial={{ opacity: 0, scale: 0.5 }}
        animate={{ opacity: 1, scale: 1 }}
        exit={{ opacity: 0, scale: 0.5 }}
        className="bg-purple-600 text-white rounded-full shadow-md flex items-center justify-center w-full h-full"
      >
        {node.value}
      </motion.div>

      {/* 4) External label for index */}
      <div
        style={{
          position: "absolute",
          left: labelX - x,
          top: labelY - y,
          transform: "translate(-50%, -50%)",
          backgroundColor: "rgba(0,0,0,0.7)",
          color: "#fff",
          padding: "3px 8px",
          borderRadius: "4px",
          fontSize: "0.8rem",
          pointerEvents: "none",
          whiteSpace: "nowrap",
        }}
      >
        {labelText}
      </div>
    </div>
  );
}

export default CircularNode;
