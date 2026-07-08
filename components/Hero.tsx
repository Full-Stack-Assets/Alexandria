import React from 'react';

const REPO_URL = 'https://github.com/Full-Stack-Assets/Alexandria';

const Hero: React.FC = () => (
  <section className="hero" id="top">
    <div className="container">
      <span className="hero__eyebrow">A modular archive</span>
      <h1 className="hero__title">Alexandria</h1>
      <p className="hero__subtitle">
        One repository, several crafts. Alexandria collects independent modules — a web app and an
        Unreal Engine world-building plugin among them — under a single roof, each buildable and
        deployable on its own.
      </p>
      <div className="hero__actions">
        <a className="button button--primary" href="#modules">
          Explore the modules
        </a>
        <a className="button button--secondary" href={REPO_URL} target="_blank" rel="noreferrer">
          View source
        </a>
      </div>
    </div>
  </section>
);

export default Hero;
