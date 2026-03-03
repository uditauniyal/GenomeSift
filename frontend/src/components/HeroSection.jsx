import React from 'react';

export default function HeroSection({ onStart, onQuickDemo }) {
    return (
        <section className="relative w-full py-12 lg:py-20 flex flex-col items-center text-center rounded-2xl border border-bio-border bg-gradient-to-b from-bio-card to-bio-bg overflow-hidden shadow-2xl mb-12">
            {/* Background decoration */}
            <div className="absolute top-0 left-0 w-full h-full opacity-10 pointer-events-none"
                style={{
                    backgroundImage: 'radial-gradient(circle at 50% 100%, #00d4ff 0%, transparent 50%)',
                    backgroundSize: '100% 200%'
                }}>
            </div>

            <div className="relative z-10 max-w-3xl px-6">
                <h2 className="text-4xl md:text-5xl font-extrabold text-white tracking-tight mb-6">
                    Decode Genetic Mutations in <span className="text-bio-cyan drop-shadow-[0_0_10px_rgba(0,212,255,0.5)]">Seconds</span>
                </h2>

                <p className="text-lg md:text-xl text-slate-400 mb-10 leading-relaxed font-light">
                    Upload DNA sequences, align them using classical bioinformatics algorithms, and detect disease-linked mutations — powered by high-performance C++.
                </p>

                <div className="flex flex-col sm:flex-row gap-5 justify-center">
                    <button
                        onClick={onQuickDemo}
                        className="px-8 py-3.5 rounded-lg bg-bio-cyan text-bio-bg font-bold text-lg hover:bg-white hover:shadow-[0_0_20px_rgba(0,212,255,0.6)] transition-all duration-300 transform hover:-translate-y-1"
                    >
                        ▶ Quick Demo
                    </button>

                    <button
                        onClick={onStart}
                        className="px-8 py-3.5 rounded-lg bg-transparent border-2 border-slate-600 text-slate-300 font-bold text-lg hover:border-white hover:text-white transition-all duration-300"
                    >
                        Start Fresh Analysis
                    </button>
                </div>
            </div>
        </section>
    );
}
