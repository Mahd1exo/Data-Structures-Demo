// src/components/Navbar.js

import React from "react";
import { FaServer } from "react-icons/fa";

function Navbar() {
  return (
    <nav className="bg-blue-600 text-white py-4 shadow-lg mb-4">
      <div className="container mx-auto px-4 flex items-center">
        <FaServer className="mr-2 text-2xl" />
        <span className="text-2xl font-bold">
          Data Structures Demo
        </span>
      </div>
    </nav>
  );
}

export default Navbar;
