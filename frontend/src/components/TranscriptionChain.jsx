import React from 'react';

export default function TranscriptionChain({ transcription }) {
    const dnaTrunc = transcription.dna.substring(0, 50) + (transcription.dna.length > 50 ? '...' : '');
    const rnaTrunc = transcription.rna.substring(0, 50) + (transcription.rna.length > 50 ? '...' : '');
    const protTrunc = transcription.protein.substring(0, 20) + (transcription.protein.length > 20 ? '...' : '');

    // Highlight T's in DNA and U's in RNA
    const renderDNA = () => {
        return dnaTrunc.split('').map((c, i) =>
            c === 'T' ? <span key={i} className="text-bio-red font-bold">{c}</span> : c
        );
    };

    const renderRNA = () => {
        return rnaTrunc.split('').map((c, i) =>
            c === 'U' ? <span key={i} className="text-bio-cyan font-bold">{c}</span> : c
        );
    };

    return (
        <div className="bg-bio-card/80 border border-bio-border rounded-xl p-8 shadow-xl">
            <h3 className="text-2xl font-bold text-white mb-2">Central Dogma Simulation</h3>
            <p className="text-slate-400 mb-8 text-sm">
                GenomeSift synthesizes RNA from the reference DNA and translates it into a protein sequence automatically.
            </p>

            <div className="flex flex-col md:flex-row items-stretch justify-between gap-4">

                {/* DNA */}
                <div className="flex-1 bg-bio-bg border border-bio-cyan/20 rounded-lg p-5 relative overflow-hidden">
                    <div className="absolute top-0 right-0 w-16 h-16 bg-bio-cyan/5 rounded-bl-full flex items-start justify-end p-3">🧬</div>
                    <h4 className="text-bio-cyan font-bold mb-3">DNA Sequence</h4>
                    <p className="font-mono text-sm text-slate-300 break-all leading-relaxed">
                        {renderDNA()}
                    </p>
                </div>

                {/* Arrow 1 */}
                <div className="flex flex-col items-center justify-center text-slate-500 py-4 md:py-0 md:px-2">
                    <div className="text-xs uppercase font-bold tracking-widest text-bio-cyan mb-1 hidden md:block rotate-90 w-24 text-center absolute -mt-16 -ml-6">Transcription</div>
                    <p className="text-xs uppercase font-bold tracking-widest text-bio-cyan mb-1 md:hidden">Transcription (T → U)</p>
                    <div className="w-1 h-8 md:w-8 md:h-1 bg-gradient-to-b md:bg-gradient-to-r from-bio-cyan/50 to-bio-green/50 animate-pulse rounded-full"></div>
                </div>

                {/* RNA */}
                <div className="flex-1 bg-bio-bg border border-bio-green/20 rounded-lg p-5 relative overflow-hidden">
                    <div className="absolute top-0 right-0 w-16 h-16 bg-bio-green/5 rounded-bl-full flex items-start justify-end p-3">📝</div>
                    <h4 className="text-bio-green font-bold mb-3">RNA Sequence</h4>
                    <p className="font-mono text-sm text-slate-300 break-all leading-relaxed">
                        {renderRNA()}
                    </p>
                </div>

                {/* Arrow 2 */}
                <div className="flex flex-col items-center justify-center text-slate-500 py-4 md:py-0 md:px-2 relative">
                    <div className="text-xs uppercase font-bold tracking-widest text-bio-purple mb-1 hidden md:block rotate-90 w-24 text-center absolute -mt-16 -ml-6">Translation</div>
                    <p className="text-xs uppercase font-bold tracking-widest text-bio-purple mb-1 md:hidden">Translation (Codons)</p>
                    <div className="w-1 h-8 md:w-8 md:h-1 bg-gradient-to-b md:bg-gradient-to-r from-bio-green/50 to-bio-purple/50 animate-pulse rounded-full"></div>
                </div>

                {/* Protein */}
                <div className="flex-1 bg-bio-bg border border-bio-purple/20 rounded-lg p-5 relative overflow-hidden glow-purple shadow-[0_0_15px_rgba(167,139,250,0.1)]">
                    <div className="absolute top-0 right-0 w-16 h-16 bg-bio-purple/5 rounded-bl-full flex items-start justify-end p-3">⚙️</div>
                    <h4 className="text-bio-purple font-bold mb-3">Protein Chain</h4>
                    <p className="font-mono text-sm text-slate-300 break-all leading-relaxed tracking-widest">
                        {protTrunc}
                    </p>
                    <div className="mt-4 flex gap-1 flex-wrap">
                        {/* Visual protein dots */}
                        {protTrunc.replace('...', '').split('').slice(0, 8).map((aa, i) => (
                            <div key={i} className="w-5 h-5 rounded-full bg-bio-purple/30 border border-bio-purple text-[10px] flex items-center justify-center text-white" title={`Amino Acid: ${aa}`}>
                                {aa}
                            </div>
                        ))}
                        <div className="w-5 h-5 flex items-center justify-center text-bio-purple text-xs">...</div>
                    </div>
                </div>
            </div>
        </div>
    );
}
