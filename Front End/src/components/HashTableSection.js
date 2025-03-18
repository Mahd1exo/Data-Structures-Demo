// src/components/HashTableSection.js
import React, { useState, useEffect } from "react";
import {
  fetchHash,
  hashInsert,
  hashRemove,
  hashContains,
} from "../services/api";
import Tilt from "react-parallax-tilt";
import { AnimatePresence, motion } from "framer-motion";
import {
  FaPlus,
  FaTimes,
  FaSearch,
  FaTrash,
  FaInfoCircle,
} from "react-icons/fa";

// Generate a unique ID for each key-value pair if needed
function genId() {
  return Date.now().toString(36) + Math.random().toString(36).substring(2);
}

function HashTableSection() {
  // Insertion states
  const [insertKey, setInsertKey] = useState("");
  const [insertValue, setInsertValue] = useState("");
  // Removal state
  const [removeKey, setRemoveKey] = useState("");
  // Contains state
  const [containsKey, setContainsKey] = useState("");
  const [containsResult, setContainsResult] = useState(null);
  // Data from backend (each element is a string "key:val")
  const [pairs, setPairs] = useState([]);
  const [sizeResult, setSizeResult] = useState(0);
  // Hash calculation display
  const [hashCalculation, setHashCalculation] = useState("");
  // Global error message for validations
  const [errorMessage, setErrorMessage] = useState("");
  // Info modal toggle
  const [showInfoModal, setShowInfoModal] = useState(false);

  useEffect(() => {
    refreshHash();
  }, []);

  async function refreshHash() {
    try {
      const data = await fetchHash();
      const arr = data.hash || [];
      setPairs(arr);
      setSizeResult(arr.length);
    } catch (err) {
      console.error("Error fetching hash table:", err);
    }
  }

  // Compute hash details: sum of ASCII codes and modulo 16
  function computeHashDetails(str) {
    let sum = 0;
    let details = "";
    for (let i = 0; i < str.length; i++) {
      const code = str.charCodeAt(i);
      sum += code;
      details += code + (i < str.length - 1 ? " + " : "");
    }
    const mod = sum % 16;
    return `${details} = ${sum}, ${sum} % 16 = ${mod}`;
  }

  // Handle input changes for insertion
  function handleInsertKeyChange(e) {
    const val = e.target.value;
    setInsertKey(val);
    setHashCalculation(val ? computeHashDetails(val) : "");
  }
  function handleInsertValueChange(e) {
    setInsertValue(e.target.value);
  }

  // Handle Insert with validations:
  async function handleInsert() {
    const key = insertKey.trim();
    const value = insertValue.trim();
    if (!key) {
      setErrorMessage("Please enter a valid key.");
      return;
    }
    if (key.length > 16) {
      setErrorMessage("Key must be 16 characters or less.");
      return;
    }
    // Check if key already exists in current pairs
    if (pairs.some((pair) => pair.split(":")[0] === key)) {
      setErrorMessage("Key already exists.");
      return;
    }
    if (!value) {
      setErrorMessage("Please enter a valid value.");
      return;
    }
    setErrorMessage("");
    try {
      await hashInsert(key, value);
      setInsertKey("");
      setInsertValue("");
      setHashCalculation("");
      setContainsResult(null);
      refreshHash();
    } catch (err) {
      console.error("Error inserting into hash table:", err);
    }
  }

  // Handle removal with validations:
  function handleRemoveKeyChange(e) {
    setRemoveKey(e.target.value);
  }
  async function handleRemove() {
    const key = removeKey.trim();
    if (!key) {
      setErrorMessage("Please enter a valid key to remove.");
      return;
    }
    if (key.length > 16) {
      setErrorMessage("Key must be 16 characters or less.");
      return;
    }
    if (!pairs.some((pair) => pair.split(":")[0] === key)) {
      setErrorMessage("Key does not exist.");
      return;
    }
    setErrorMessage("");
    try {
      await hashRemove(key);
      setRemoveKey("");
      refreshHash();
    } catch (err) {
      console.error("Error removing from hash table:", err);
    }
  }

  // Handle checking for key existence with validations:
  function handleContainsKeyChange(e) {
    setContainsKey(e.target.value);
    setContainsResult(null);
  }
  async function handleContains() {
    const key = containsKey.trim();
    if (!key) {
      setErrorMessage("Please enter a valid key to check.");
      return;
    }
    if (key.length > 16) {
      setErrorMessage("Key must be 16 characters or less.");
      return;
    }
    setErrorMessage("");
    try {
      const res = await hashContains(key);
      setContainsResult(res.contains ? "true" : "false");
    } catch (err) {
      console.error("Error checking contains:", err);
    }
  }

  // Build buckets: each key-value pair goes to bucket index = (sum of key ASCII codes) % 16
  function buildBuckets() {
    const buckets = Array.from({ length: 16 }, () => []);
    pairs.forEach((pairStr) => {
      const [k, v] = pairStr.split(":");
      let sum = 0;
      for (let i = 0; i < k.length; i++) {
        sum += k.charCodeAt(i);
      }
      const index = sum % 16;
      buckets[index].push({ key: k, val: v });
    });
    return buckets;
  }
  const buckets = buildBuckets();

  function toggleInfoModal() {
    setShowInfoModal((prev) => !prev);
  }

  return (
    <div className="w-full p-6">
      {/* Header: Left-Aligned Title, Right-Aligned Info Button */}
      <div className="flex justify-between items-center mb-4">
        <h2 className="text-3xl font-bold text-gray-800">Hash Table Manager</h2>
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
                About Hash Tables
              </h3>
              <p className="text-gray-700 text-base leading-relaxed mb-4">
                A <strong>hash table</strong> stores key-value pairs and uses a hash
                function to compute an index into an array of buckets. It offers efficient
                lookups, insertions, and deletions. In this implementation, the bucket
                for each key is determined by summing the ASCII codes of its characters and
                taking the modulo with 16.
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

      {/* Insertion Section */}
      <div className="mb-4 p-4 border rounded-md shadow-sm bg-white">
        <h3 className="text-lg font-medium text-gray-700 border-b pb-1 mb-3">
          Insert Key-Value Pair
        </h3>
        <div className="flex items-center gap-4">
          <input
            type="text"
            placeholder="Key"
            className="flex-1 border border-gray-300 p-2 rounded focus:outline-none focus:ring focus:ring-blue-300"
            value={insertKey}
            onChange={handleInsertKeyChange}
          />
          <input
            type="text"
            placeholder="Value"
            className="flex-1 border border-gray-300 p-2 rounded focus:outline-none focus:ring focus:ring-blue-300"
            value={insertValue}
            onChange={handleInsertValueChange}
          />
          <button
            className="flex items-center gap-1 bg-blue-500 hover:bg-blue-600 text-white px-4 py-2 rounded text-sm font-semibold transition-colors duration-300"
            onClick={handleInsert}
          >
            <FaPlus />
            Insert
          </button>
          <button
            className="flex items-center gap-1 bg-gray-500 hover:bg-gray-600 text-white px-4 py-2 rounded text-sm font-semibold transition-colors duration-300"
            onClick={() => {
              setInsertKey("");
              setInsertValue("");
              setHashCalculation("");
            }}
          >
            <FaTimes />
            Clear
          </button>
        </div>
        {hashCalculation && (
          <div className="mt-2 text-sm text-gray-500">
            <strong>Hash Calculation:</strong> {hashCalculation}
          </div>
        )}
      </div>

      {/* Combined Check & Remove Section */}
      <div className="mb-4 p-4 border rounded-md shadow-sm bg-white">
        <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
          {/* Check Key Existence */}
          <div className="flex flex-col">
            <h3 className="text-lg font-medium text-gray-700 border-b pb-1 mb-3">
              Check Key Existence
            </h3>
            <div className="flex items-center gap-2">
              <input
                type="text"
                placeholder="Key to check"
                className="flex-1 border border-gray-300 p-2 rounded focus:outline-none focus:ring focus:ring-blue-300"
                value={containsKey}
                onChange={handleContainsKeyChange}
              />
              <button
                className="flex items-center gap-1 bg-gray-500 hover:bg-gray-600 text-white px-4 py-2 rounded text-sm font-semibold transition-colors duration-300"
                onClick={handleContains}
              >
                <FaSearch />
                Check
              </button>
            </div>
            {containsResult !== null && (
              <span className="mt-2 text-lg font-medium text-gray-700">
                Result: {containsResult}
              </span>
            )}
          </div>
          {/* Remove Key */}
          <div className="flex flex-col">
            <h3 className="text-lg font-medium text-gray-700 border-b pb-1 mb-3">
              Remove Key
            </h3>
            <div className="flex items-center gap-2">
              <input
                type="text"
                placeholder="Key to remove"
                className="flex-1 border border-gray-300 p-2 rounded focus:outline-none focus:ring focus:ring-red-300"
                value={removeKey}
                onChange={(e) => setRemoveKey(e.target.value)}
              />
              <button
                className="flex items-center gap-1 bg-red-500 hover:bg-red-600 text-white px-4 py-2 rounded text-sm font-semibold transition-colors duration-300"
                onClick={handleRemove}
              >
                <FaTrash />
                Remove
              </button>
            </div>
          </div>
        </div>
      </div>

      {/* Size Info */}
      <div className="mb-4 p-4 border rounded-md shadow-sm bg-white">
        <span className="text-md font-medium text-gray-700">
          Total Keys: {sizeResult}
        </span>
      </div>

      {/* Buckets Display */}
      <div className="p-4 border rounded-md bg-gray-100 shadow-sm">
        <h3 className="text-lg font-medium text-gray-700 border-b pb-1 mb-3">
          Buckets
        </h3>
        <div className="grid grid-cols-1 sm:grid-cols-2 md:grid-cols-4 gap-4">
          {buckets.map((bucket, idx) => (
            <Tilt
              key={idx}
              glareEnable={true}
              glareMaxOpacity={0.2}
              scale={1.02}
              transitionSpeed={250}
              tiltMaxAngleX={5}
              tiltMaxAngleY={5}
              className="p-3 border rounded bg-gray-200"
            >
              <div className="flex justify-between items-center mb-2">
                <span className="font-medium text-gray-800">
                  Bucket {idx}:
                </span>
                <span className="text-sm text-gray-500">Key : Value</span>
              </div>
              {bucket.length > 0 ? (
                bucket.map((item, i) => (
                  <div
                    key={i}
                    className="flex items-center mb-1 p-2 bg-white border border-gray-300 rounded"
                  >
                    <span className="flex-1 text-left text-sm text-gray-700">
                      {item.key}
                    </span>
                    <span className="mx-2 text-center text-sm text-gray-500">
                      :
                    </span>
                    <span className="flex-1 text-right text-sm text-gray-700">
                      {item.val}
                    </span>
                  </div>
                ))
              ) : (
                <div className="text-gray-400 text-sm">Empty</div>
              )}
            </Tilt>
          ))}
        </div>
      </div>
    </div>
  );
}

export default HashTableSection;
