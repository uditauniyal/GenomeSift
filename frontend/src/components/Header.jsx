import React from 'react';

export default function Header() {
    return (
        <header className="fixed top-0 inset-x-0 z-50 bg-bio-bg/80 backdrop-blur-md border-b border-bio-cyan/20 h-16 flex items-center justify-between px-6">
            <div className="flex items-center gap-3">
                {/* DNA SVG Icon */}
                <div className="w-8 h-8 text-bio-cyan animate-[spin_10s_linear_infinite]">
                    <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
                        <path d="M4 4v16M20 4v16M4 8h16M4 16h16M8 4c0 3-4 6-4 8s4 5 4 8M16 4c0 3 4 6 4 8s-4 5-4 8" className="opacity-70" />
                    </svg>
                </div>
                <h1 className="text-xl font-bold tracking-tight text-white m-0">
                    Genome<span className="text-bio-cyan">Sift</span>
                </h1>
            </div>

            <div className="hidden md:block text-slate-400 font-medium text-sm tracking-wide">
                DNA Sequence Alignment & Mutation Detection Engine
            </div>

            <div className="flex items-center">
                <span className="px-3 py-1 rounded-full bg-bio-cyan/10 border border-bio-cyan/30 text-bio-cyan text-xs font-semibold uppercase tracking-wider glow-cyan">
                    C++ Core Active
                </span>
            </div>
        </header>
    );
}
