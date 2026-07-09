import React from 'react';

interface ModuleInfo {
  tag: string;
  title: string;
  description: string;
  stack: string[];
  href: string;
}

const MODULES: ModuleInfo[] = [
  {
    tag: 'Web',
    title: 'Alexandria Web',
    description:
      'The site you are looking at right now — a Vite + React + TypeScript app wired for the Gemini API, deployed straight from this repository.',
    stack: ['Vite', 'React', 'TypeScript', 'Gemini API'],
    href: 'https://github.com/Full-Stack-Assets/Alexandria/tree/main',
  },
  {
    tag: 'Unreal Engine 5.8',
    title: 'Alexandria Library Plugin',
    description:
      'A procedural greybox generator for a walkable Library of Alexandria: a domed central atrium, six ringed halls, and an optional underground chamber grid — packaged as a drop-in UE plugin.',
    stack: ['C++', 'Unreal Engine', 'Procedural Geometry'],
    href: 'https://github.com/Full-Stack-Assets/Alexandria/tree/main/unreal/AlexandriaLibrary',
  },
];

const Modules: React.FC = () => (
  <section className="modules" id="modules">
    <div className="container">
      <div className="modules__heading">
        <h2>What's inside</h2>
        <p>Each module lives in its own corner of the repo and ships independently.</p>
      </div>
      <div className="modules__grid">
        {MODULES.map((mod) => (
          <article className="module-card" key={mod.title}>
            <span className="module-card__tag">{mod.tag}</span>
            <h3 className="module-card__title">{mod.title}</h3>
            <p className="module-card__desc">{mod.description}</p>
            <div className="module-card__stack">
              {mod.stack.map((item) => (
                <span className="module-card__pill" key={item}>
                  {item}
                </span>
              ))}
            </div>
            <a className="module-card__link" href={mod.href} target="_blank" rel="noopener noreferrer">
              Browse module →
            </a>
          </article>
        ))}
      </div>
    </div>
  </section>
);

export default Modules;
