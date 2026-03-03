import React, { useState, useRef, useEffect } from 'react';

export default function AlignmentVisualization({ alignment, mutations }) {
    const [windowStart, setWindowStart] = useState(0);
    const containerRef = useRef(null);

    const s1 = alignment.aligned_seq1 || '';
    const s2 = alignment.aligned_seq2 || '';
    const len = Math.max(s1.length, s2.length);

    // Calculate match line
    let matchLine = '';
    for (let i = 0; i < len; i++) {
        const c1 = s1[i] || ' ';
        const c2 = s2[i] || ' ';
        if (c1 === '-' || c2 === '-') matchLine += ' ';
        else if (c1 === c2) matchLine += '|';
        else matchLine += '×';
    }

    // Pre-calculate mutation positions mapped to alignment index
    const mutIndices = new Set();
    mutations.forEach(m => {
        // Attempting to highlight exact alignment indices. 
        // m.position is the 0-based index in the original reference seq without gaps.
        // We need to map reference position -> alignment index
        let refIdx = 0;
        for (let i = 0; i < len; i++) {
            if (s1[i] !== '-') {
                if (refIdx === m.position) {
                    mutIndices.add(i);
                    break;
                }
                refIdx++;
            }
        }
    });

    const jumpToMutation = (m) => {
        let refIdx = 0;
        let targetAlignIdx = 0;
        for (let i = 0; i < len; i++) {
            if (s1[i] !== '-') {
                if (refIdx === m.position) {
                    targetAlignIdx = i;
                    break;
                }
                refIdx++;
            }
        }

        if (containerRef.current) {
            // rough estimation for scroll position (each char is ~12px wide normally)
            // We will just try to scroll by ratio
            const ratio = targetAlignIdx / len;
            containerRef.current.scrollLeft = ratio * containerRef.current.scrollWidth - (containerRef.current.clientWidth / 2);
        }
    };

    return (
        <div className="bg-bio-bg border border-bio-border rounded-xl p-6 shadow-lg">
            <div className="flex justify-between items-center mb-6">
                <div>
                    <h3 className="text-2xl font-bold text-white mb-1">Sequence Alignment</h3>
                    <p className="text-sm text-slate-400">
                        Hover over bases or scroll horizontally to inspect the alignment.
                    </p>
                </div>

                {mutations.length > 0 && (
                    <div className="flex gap-2">
                        {mutations.map((m, i) => (
                            <button
                                key={i}
                                onClick={() => jumpToMutation(m)}
                                className="text-xs bg-bio-red/10 border border-bio-red/30 text-bio-red px-2 py-1 rounded hover:bg-bio-red/20 transition"
                            >
                                Jump to {m.type} @ {m.position}
                            </button>
                        ))}
                    </div>
                )}
            </div>

            {/* Mini-map */}
            <div className="w-full h-8 bg-black/50 border border-bio-border rounded mb-4 relative overflow-hidden flex">
                {matchLine.split('').map((char, i) => {
                    let bg = 'bg-bio-green/40';
                    if (char === '×') bg = 'bg-bio-red';
                    if (char === ' ') bg = 'bg-bio-amber/50';
                    return <div key={i} className={`h-full flex-1 ${bg}`}></div>;
                })}
            </div>

            {/* Sequence container */}
            <div
                ref={containerRef}
                className="w-full overflow-x-auto bg-bio-card/50 rounded-lg border border-black p-4 hide-scrollbar"
                style={{ scrollBehavior: 'smooth' }}
            >
                <div className="inline-block min-w-full font-mono text-base tracking-[0.2em] whitespace-nowrap">
                    {/* Markers Row */}
                    <div className="flex h-6 items-end mb-1">
                        <div className="w-32 flex-shrink-0 text-slate-500 text-xs text-right pr-4 pb-1">Markers</div>
                        <div className="flex">
                            {Array.from({ length: len }).map((_, i) => (
                                <div key={i} className="w-[1.2em] flex justify-center">
                                    {mutIndices.has(i) ? (
                                        <span className="text-bio-red text-lg drop-shadow-[0_0_5px_rgba(255,71,87,0.8)] animate-bounce font-sans">▼</span>
                                    ) : ' '}
                                </div>
                            ))}
                        </div>
                    </div>

                    {/* Reference Sequence */}
                    <div className="flex bg-black/40 py-1 rounded">
                        <div className="w-32 flex-shrink-0 text-bio-cyan font-bold text-xs flex items-center justify-end pr-4 uppercase tracking-normal">Reference</div>
                        <div className="flex">
                            {s1.split('').map((char, i) => {
                                const isMut = mutIndices.has(i);
                                return (
                                    <div key={i} className={`w-[1.2em] flex justify-center items-center ${isMut ? 'bg-bio-red/20 text-bio-red font-bold' : (char === '-' ? 'text-slate-500' : 'text-slate-200')}`}>
                                        {char}
                                    </div>
                                );
                            })}
                        </div>
                    </div>

                    {/* Match Line */}
                    <div className="flex my-1">
                        <div className="w-32 flex-shrink-0"></div>
                        <div className="flex text-slate-500/50">
                            {matchLine.split('').map((char, i) => (
                                <div key={i} className={`w-[1.2em] flex justify-center items-center ${char === '×' ? 'text-bio-red font-bold' : (char === '|' ? 'text-bio-green/50' : '')}`}>
                                    {char}
                                </div>
                            ))}
                        </div>
                    </div>

                    {/* Patient Sequence */}
                    <div className="flex bg-black/40 py-1 rounded pb-2">
                        <div className="w-32 flex-shrink-0 text-[#a78bfa] font-bold text-xs flex items-center justify-end pr-4 uppercase tracking-normal">Patient</div>
                        <div className="flex">
                            {s2.split('').map((char, i) => {
                                const isMut = mutIndices.has(i);
                                let bgClass = '';
                                let textClass = 'text-slate-200';

                                if (char === '-') {
                                    bgClass = 'bg-bio-amber/20';
                                    textClass = 'text-bio-amber';
                                } else if (matchLine[i] === '×' || isMut) {
                                    bgClass = 'bg-bio-red/30 border-y border-bio-red';
                                    textClass = 'text-bio-red font-bold';
                                } else if (matchLine[i] === '|') {
                                    bgClass = 'bg-bio-green/10';
                                    textClass = 'text-bio-green font-light';
                                }

                                return (
                                    <div key={i} className={`w-[1.2em] flex justify-center items-center ${bgClass} ${textClass}`}>
                                        {char}
                                    </div>
                                );
                            })}
                        </div>
                    </div>
                </div>
            </div>

            <div className="flex items-center gap-6 mt-4 text-xs font-semibold text-slate-400 justify-center">
                <div className="flex items-center gap-2"><div className="w-3 h-3 rounded bg-bio-green/20 border border-bio-green"></div> Match</div>
                <div className="flex items-center gap-2"><div className="w-3 h-3 rounded bg-bio-red/30 border border-bio-red"></div> Mismatch (SNP)</div>
                <div className="flex items-center gap-2"><div className="w-3 h-3 rounded bg-bio-amber/20 border border-bio-amber"></div> Gap (InDel)</div>
            </div>
        </div>
    );
}
