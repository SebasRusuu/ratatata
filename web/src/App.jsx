import { useState } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './App.css'
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom'
import Controller from './pages/controller'
import MainPage from './pages/mainpage'

function App() {

  return (
    <Router>
      <Routes>
        <Route path="/" element={<MainPage />} />
        <Route path="/controller" element={<Controller />} />
      </Routes>
    </Router>
  )
}

export default App
