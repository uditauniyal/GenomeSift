import React from 'react';
import StatsCards from './StatsCards';
import AlignmentVisualization from './AlignmentVisualization';
import MutationCards from './MutationCards';
import TranscriptionChain from './TranscriptionChain';
import DPMatrixHeatmap from './DPMatrixHeatmap';

export default function Step3Results({ results, onReset }) {
    if (!results) return null;

    const { alignment, mutations, transcription, summary } = results;

    const handleDownload = async () => {
        try {
            const res = await fetch('/api/report', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify(results)
            });
            const data = await res.json();
            if (data.download_url) {
                window.open(data.download_url, '_blank');
            } else {
                alert("Failed to generate report");
            }
        } catch (e) {
            console.error(e);
            alert("Error generating report");
        }
    };

    return (
        <div className="space-y-12 animate-in fade-in slide-in-from-bottom-8 duration-700">

            <div className="flex items-center justify-between mb-8">
                <div className="flex items-center gap-4">
                    <div className="flex items-center justify-center w-10 h-10 rounded-full bg-bio-cyan/20 border border-bio-cyan text-bio-cyan font-bold text-xl glow-cyan">
                        3
                    </div>
                    <h2 className="text-3xl font-bold">Analysis Results</h2>
                </div>
            </div>

            {/* 3A: ALIGNMENT OVERVIEW */}
            <StatsCards alignment={alignment} summary={summary} />

            {/* 3B: ALIGNMENT VISUALIZATION */}
            <AlignmentVisualization alignment={alignment} mutations={mutations} />

            {/* 3C: MUTATIONS DETECTED */}
            <MutationCards mutations={mutations} summary={summary} />

            {/* 3D: TRANSCRIPTION CHAIN */}
            {transcription && (
                <TranscriptionChain transcription={transcription} />
            )}

            {/* 3E: DP MATRIX HEATMAP (if available and small enough) */}
            {alignment.dp_matrix && alignment.dp_matrix.length > 0 && !alignment.dp_matrix_truncated && (
                <DPMatrixHeatmap matrix={alignment.dp_matrix} seq1={alignment.aligned_seq1} seq2={alignment.aligned_seq2} />
            )}

            {/* REPORT ACTIONS / FOOTER */}
            <div className="pt-12 flex flex-col sm:flex-row items-center justify-center gap-6 border-t border-bio-border mt-16">
                <button
                    onClick={handleDownload}
                    className="px-8 py-3 rounded-lg bg-bio-cyan/10 border border-bio-cyan text-bio-cyan font-bold hover:bg-bio-cyan hover:text-bio-bg transition-all w-full sm:w-auto flex items-center justify-center gap-2"
                >
                    📄 Download Full Report
                </button>

                <button
                    onClick={onReset}
                    className="px-8 py-3 rounded-lg bg-bio-card border border-slate-600 text-slate-300 font-bold hover:text-white hover:border-white transition-all w-full sm:w-auto"
                >
                    ↺ Run New Analysis
                </button>

                <button
                    disabled
                    className="px-8 py-3 rounded-lg bg-slate-800 border border-slate-700 text-slate-500 font-bold cursor-not-allowed w-full sm:w-auto"
                >
                    Share Results (Coming Soon)
                </button>
            </div>

        </div>
    );
}
