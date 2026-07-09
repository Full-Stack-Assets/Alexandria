import React from 'react';
import Header from './components/Header';
import Hero from './components/Hero';
import Modules from './components/Modules';
import Footer from './components/Footer';
import './App.css';

const App: React.FC = () => (
  <>
    <Header />
    <main>
      <Hero />
      <Modules />
    </main>
    <Footer />
  </>
);

export default App;
