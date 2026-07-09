import React from 'react';

const REPO_URL = 'https://github.com/Full-Stack-Assets/Alexandria';

const Header: React.FC = () => (
  <header className="site-header">
    <div className="container site-header__bar">
      <a href="#top" className="site-header__brand">
        <span className="site-header__brand-mark">Alexandria</span>
      </a>
      <nav className="site-header__nav">
        <a href="#modules">Modules</a>
        <a href={REPO_URL} target="_blank" rel="noopener noreferrer">
          GitHub
        </a>
      </nav>
    </div>
  </header>
);

export default Header;
