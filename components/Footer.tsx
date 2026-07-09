import React from 'react';

const REPO_URL = 'https://github.com/Full-Stack-Assets/Alexandria';

const Footer: React.FC = () => (
  <footer className="site-footer">
    <div className="container site-footer__bar">
      <span>&copy; {new Date().getFullYear()} Alexandria</span>
      <a href={REPO_URL} target="_blank" rel="noopener noreferrer">
        Full-Stack-Assets/Alexandria
      </a>
    </div>
  </footer>
);

export default Footer;
