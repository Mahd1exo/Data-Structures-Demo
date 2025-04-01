import React, { useState, useEffect, useCallback } from "react";
import { AnimatePresence, motion } from "framer-motion";
import "../util/InformationSection.css"; // Assuming you have a CSS file for styles

const SLIDE_DATA = [
  {
    title: "Introduction & Setup",
    description: "Our backend is developed in C using CivetWeb, leveraging efficient data structures to boost performance.\n\nWe use a <strong>Task Queue</strong> for asynchronous tasks, a <strong>Stack</strong> for strict ordering, and a <strong>Thread Pool</strong> for parallel execution. These patterns optimize <strong>resource management</strong>, <strong>error handling</strong>, and <strong>dynamic scheduling</strong>.\n\nThis presentation summarizes each pattern and explains why I chose it.",
    bullets: []
  },
  {
    title: "Task Queue (FIFO)",
    description: "A <strong>Task Queue</strong> manages asynchronous tasks by enqueuing them for later processing.",
    bullets: [
      "<strong>Decouples Execution:</strong> Separates task scheduling from processing.",
      "<strong>Centralized Management:</strong> One location for logging and error handling.",
      "<strong>Flexible Ordering:</strong> Supports FIFO, priority, or multi-level queues.",
      "<strong>Scalable:</strong> Multiple consumers can process tasks concurrently.",
      "<strong>Asynchronous:</strong> Producers do not wait for tasks to complete."
    ],
    advancedBullets: [
      {
        heading: "Why I Used a Task Queue",
        points: [
          "Keeps the <strong>main flow responsive</strong>.",
          "Simplifies <strong>monitoring</strong> and <strong>error handling</strong>.",
          "Easily adapts to various <strong>ordering schemes</strong>.",
          "Enhances <strong>scalability</strong> with parallel consumers."
        ]
      }
    ]
  },
  {
    title: "Thread Pool",
    description: "A <strong>Thread Pool</strong> reuses a fixed set of threads to run tasks concurrently, reducing overhead.",
    bullets: [
      "<strong>Efficient Parallelism:</strong> Reuses threads for multiple tasks.",
      "<strong>Resource Control:</strong> Limits active threads based on system capacity.",
      "<strong>Improved Throughput:</strong> Executes many short tasks concurrently.",
      "<strong>Reusability:</strong> Amortizes the cost of thread creation.",
      "<strong>Load Balancing:</strong> Evenly distributes work among threads."
    ],
    advancedBullets: [
      {
        heading: "Why I Used a Thread Pool",
        points: [
          "Minimizes <strong>overhead</strong> by avoiding frequent thread creation.",
          "Controls <strong>resource usage</strong> effectively.",
          "Boosts <strong>performance</strong> under heavy load.",
          "Ensures <strong>predictable</strong> execution behavior."
        ]
      }
    ]
  },
  {
    title: "Stack (LIFO) Tasks",
    description: "A <strong>Stack (LIFO)</strong> processes tasks in reverse order—ideal for strict sequential operations.",
    bullets: [
      "<strong>Reverse Ordering:</strong> Processes the most recent task first.",
      "<strong>Linear Dependencies:</strong> Ideal for ordered sequences.",
      "<strong>Minimal Overhead:</strong> Simple and efficient data structure.",
      "<strong>Clarity:</strong> Clear dependency management.",
      "<strong>Common Uses:</strong> Initialization, cleanup, and undo/redo operations."
    ],
    advancedBullets: [
      {
        heading: "Why I Used a Stack (LIFO)",
        points: [
          "Enforces <strong>strict order</strong> when dependencies are clear.",
          "Simplifies <strong>initialization</strong> and <strong>cleanup</strong> tasks.",
          "Offers <strong>fast O(1)</strong> push/pop operations.",
          "Ensures only the <strong>latest task</strong> is executed next."
        ]
      }
    ]
  },
  {
    title: "Asynchronous Design (Partial Async)",
    description: "Asynchronous programming enables tasks to run without blocking the main thread. <strong>Partial async</strong> mixes synchronous and asynchronous operations.",
    bullets: [
      "<strong>Responsive:</strong> Main flow stays active during slow tasks.",
      "<strong>Concurrent:</strong> Interleaves synchronous and asynchronous tasks.",
      "<strong>Scalable:</strong> Efficient handling of I/O-bound operations.",
      "<strong>Complexity:</strong> Adds complexity but avoids freezing."
    ],
    advancedBullets: [
      {
        heading: "Why I Used Asynchronous Design",
        points: [
          "Keeps the application <strong>responsive</strong>.",
          "Balances <strong>synchronous logic</strong> with asynchronous tasks.",
          "Optimizes <strong>I/O operations</strong> and overall scalability.",
          "Maintains a <strong>responsive main thread</strong> during long tasks."
        ]
      }
    ]
  },
  {
    title: "References",
    description: "Below are references that informed the content and design of this presentation. Thank you for reading!",
    bullets: [],
    advancedBullets: [
      {
        heading: "Citations",
        points: [
          "<strong>JavaScript Event Loop & Task Queue</strong> – MDN Web Docs",
          "<strong>Thread Pool</strong> – Wikipedia",
          "<strong>Stack (LIFO)</strong> – Various technical articles",
          "<strong>Asynchronous Programming</strong> – Node.js & concurrency frameworks",
          "<strong>Celery & Redis</strong> – FullStackPython.com"
        ]
      },
      {
        heading: "Additional References",
        points: [
          "<strong>React Three Fiber Docs</strong> – R3F.DOCS.PMND.RS",
          "<strong>3D Carousel Example</strong> – Codrops Tutorial",
          "<strong>W3C HTML5 Event Loop</strong> documentation"
        ]
      }
    ]
  }
];

function Slide({ slideData }) {
  return (
    <motion.div 
      className="slide-container"
      initial={{ opacity: 0 }}
      animate={{ opacity: 1 }}
      exit={{ opacity: 0 }}
    >
      <h2 className="slide-title">{slideData.title}</h2>
      <p className="slide-description">
        {slideData.description.split("\n").map((line, idx) => (
          <React.Fragment key={idx}>
            <span dangerouslySetInnerHTML={{ __html: line }} />
            <br />
          </React.Fragment>
        ))}
      </p>
      
      {slideData.bullets.length > 0 && (
        <motion.ul className="slide-bullets">
          {slideData.bullets.map((bullet, idx) => (
            <motion.li
              key={idx}
              initial={{ x: -20, opacity: 0 }}
              animate={{ x: 0, opacity: 1 }}
              transition={{ delay: idx * 0.1 }}
              dangerouslySetInnerHTML={{ __html: bullet }}
            />
          ))}
        </motion.ul>
      )}

      {slideData.advancedBullets?.map((section, idx) => (
        <motion.div 
          key={idx}
          className="advanced-section"
          initial={{ y: 20, opacity: 0 }}
          animate={{ y: 0, opacity: 1 }}
        >
          <h3 className="advanced-heading">{section.heading}</h3>
          <ul className="advanced-bullets">
            {section.points.map((pt, i) => (
              <li key={i} dangerouslySetInnerHTML={{ __html: pt }} />
            ))}
          </ul>
        </motion.div>
      ))}
    </motion.div>
  );
}

export default function InformationSection() {
  const [slideIndex, setSlideIndex] = useState(0);
  const [direction, setDirection] = useState(1);

  const handleNext = useCallback(() => {
    setDirection(1);
    setSlideIndex(prev => (prev + 1) % SLIDE_DATA.length);
  }, []);

  const handlePrev = useCallback(() => {
    setDirection(-1);
    setSlideIndex(prev => (prev - 1 + SLIDE_DATA.length) % SLIDE_DATA.length);
  }, []);

  useEffect(() => {
    const handleKeyDown = (e) => {
      if (e.key === "ArrowRight") handleNext();
      if (e.key === "ArrowLeft") handlePrev();
    };
    document.addEventListener("keydown", handleKeyDown);
    return () => document.removeEventListener("keydown", handleKeyDown);
  }, [handleNext, handlePrev]);

  const variants = {
    initial: (direction) => ({ 
      x: direction > 0 ? 100 : -100, 
      opacity: 0,
      scale: 0.95
    }),
    animate: { 
      x: 0, 
      opacity: 1,
      scale: 1,
      transition: { type: "spring", stiffness: 100 }
    },
    exit: (direction) => ({ 
      x: direction > 0 ? -100 : 100, 
      opacity: 0,
      scale: 0.95,
      transition: { duration: 0.2 }
    })
  };

  return (
    <div className="information-section">
      <div className="progress-indicator">
        {SLIDE_DATA.map((_, idx) => (
          <div 
            key={idx}
            className={`progress-dot ${idx === slideIndex ? 'active' : ''}`}
            onClick={() => setSlideIndex(idx)}
          />
        ))}
      </div>

      <div className="slider-container">
        <AnimatePresence mode="popLayout" custom={direction}>
          <motion.div
            key={slideIndex}
            custom={direction}
            variants={variants}
            initial="initial"
            animate="animate"
            exit="exit"
            className="slide-wrapper"
          >
            <Slide slideData={SLIDE_DATA[slideIndex]} />
          </motion.div>
        </AnimatePresence>
      </div>

      <div className="navigation">
        <motion.button 
          onClick={handlePrev}
          whileHover={{ scale: 1.1 }}
          whileTap={{ scale: 0.95 }}
          className="nav-button prev"
        >
          ←
        </motion.button>
        <motion.button 
          onClick={handleNext}
          whileHover={{ scale: 1.1 }}
          whileTap={{ scale: 0.95 }}
          className="nav-button next"
        >
          →
        </motion.button>
      </div>
    </div>
  );
}