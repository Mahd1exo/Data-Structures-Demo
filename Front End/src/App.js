// src/App.js
import React, { useState } from "react";
import { AnimatePresence, motion } from "framer-motion";

import Navbar from "./components/Navbar";
import Tabs from "./components/Tabs";

// Existing sections
import LinkedListSection from "./components/LinkedListSection";
import StackSection from "./components/StackSection";
import QueueSection from "./components/QueueSection";
import SortedListSection from "./components/SortedListSection";
import CircularListSection from "./components/CircularListSection";
import HeapSection from "./components/HeapSection";
import MinHeapSection from "./components/MinHeapSection";
import HashTableSection from "./components/HashTableSection";
import TreeSection from "./components/TreeSection";
import BSTSection from "./components/BSTSection";
import AVLSection from "./components/AVLSection";
import InformationSection from "./components/InformationSection";

function App() {
  const [activeTab, setActiveTab] = useState("list");

  const pageVariants = {
    initial: { opacity: 0, x: 50 },
    in: { opacity: 1, x: 0 },
    out: { opacity: 0, x: -50 },
  };

  return (
    <div className="min-h-screen flex flex-col bg-gray-50">
      <Navbar />

      <div className="container mx-auto px-4">
        {/* 2) Ensure your Tabs component includes an "info" tab option */}
        <Tabs activeTab={activeTab} onTabChange={setActiveTab} />

        <div className="relative border border-t-0 rounded-b bg-white p-4 shadow">
          <AnimatePresence mode="wait">
            {activeTab === "list" && (
              <motion.div
                key="list"
                initial="initial"
                animate="in"
                exit="out"
                variants={pageVariants}
                transition={{ duration: 0.3 }}
              >
                <LinkedListSection />
              </motion.div>
            )}
            {activeTab === "stack" && (
              <motion.div
                key="stack"
                initial="initial"
                animate="in"
                exit="out"
                variants={pageVariants}
                transition={{ duration: 0.3 }}
              >
                <StackSection />
              </motion.div>
            )}
            {activeTab === "queue" && (
              <motion.div
                key="queue"
                initial="initial"
                animate="in"
                exit="out"
                variants={pageVariants}
                transition={{ duration: 0.3 }}
              >
                <QueueSection />
              </motion.div>
            )}
            {activeTab === "sorted" && (
              <motion.div
                key="sorted"
                initial="initial"
                animate="in"
                exit="out"
                variants={pageVariants}
                transition={{ duration: 0.3 }}
              >
                <SortedListSection />
              </motion.div>
            )}
            {activeTab === "circular" && (
              <motion.div
                key="circular"
                initial="initial"
                animate="in"
                exit="out"
                variants={pageVariants}
                transition={{ duration: 0.3 }}
              >
                <CircularListSection />
              </motion.div>
            )}
            {activeTab === "heap" && (
              <motion.div
                key="heap"
                initial="initial"
                animate="in"
                exit="out"
                variants={pageVariants}
                transition={{ duration: 0.3 }}
              >
                <HeapSection />
              </motion.div>
            )}
            {activeTab === "minheap" && (
              <motion.div
                key="minheap"
                initial="initial"
                animate="in"
                exit="out"
                variants={pageVariants}
                transition={{ duration: 0.3 }}
              >
                <MinHeapSection />
              </motion.div>
            )}
            {activeTab === "hash" && (
              <motion.div
                key="hash"
                initial="initial"
                animate="in"
                exit="out"
                variants={pageVariants}
                transition={{ duration: 0.3 }}
              >
                <HashTableSection />
              </motion.div>
            )}
            {activeTab === "tree" && (
              <motion.div
                key="tree"
                initial="initial"
                animate="in"
                exit="out"
                variants={pageVariants}
                transition={{ duration: 0.3 }}
              >
                <TreeSection />
              </motion.div>
            )}
            {activeTab === "bst" && (
              <motion.div
                key="bst"
                initial="initial"
                animate="in"
                exit="out"
                variants={pageVariants}
                transition={{ duration: 0.3 }}
              >
                <BSTSection />
              </motion.div>
            )}
            {activeTab === "avl" && (
              <motion.div
                key="avl"
                initial="initial"
                animate="in"
                exit="out"
                variants={pageVariants}
                transition={{ duration: 0.3 }}
              >
                <AVLSection />
              </motion.div>
            )}

            {/* 3) New tab: "info" displays InformationSection */}
            {activeTab === "info" && (
              <motion.div
                key="info"
                initial="initial"
                animate="in"
                exit="out"
                variants={pageVariants}
                transition={{ duration: 0.3 }}
              >
                <InformationSection />
              </motion.div>
            )}
          </AnimatePresence>
        </div>
      </div>
    </div>
  );
}

export default App;
