import React from 'react';

export default function DPMatrixHeatmap({ matrix, seq1, seq2 }) {
    if (!matrix || matrix.length === 0) return null;

    // Find max and min for color scaling
    let max = -Infinity;
    let min = Infinity;
    matrix.forEach(row => {
        row.forEach(val => {
            if (val > max) max = val;
            if (val < min) min = val;
        });
    });

    const range = max - min;

    // Very simplistic color mapper
    const getColor = (val) => {
        if (range === 0) return 'rgba(0, 212, 255, 0.5)';
        const normalized = (val - min) / range; // 0 to 1
        // Color scale: dark blue (0) -> cyan (0.5) -> bright white (1)
        return `rgba(0, ${Math.floor(normalized * 212 + 40)}, 255, ${normalized * 0.8 + 0.2})`;
    };

    return (
        <div className="bg-bio-card/50 border border-bio-border rounded-xl p-6">
            <h3 className="text-xl font-bold text-white mb-2">Dynamic Programming Matrix (Sample)</h3>
            <p className="text-slate-400 text-sm mb-6">
                Heatmap of the scoring grid used to compute the alignment. Showing top-left 50x50 block. Brightest = highest score.
            </p>

            <div className="overflow-x-auto overflow-y-auto max-h-96 pr-4 pb-4 custom-scrollbar">
                <div className="inline-block bg-black/50 p-2 rounded">
                    <table className="border-collapse text-[8px] sm:text-[10px] font-mono text-center">
                        <tbody>
                            {/* Note: We skipped headers for simplicity in json, just showing raw matrix */}
                            {matrix.map((row, i) => (
                                <tr key={i}>
                                    {row.map((val, j) => (
                                        <td
                                            key={j}
                                            className="w-4 h-4 sm:w-6 sm:h-6 p-0 border border-black/30 text-white/70 hover:text-white"
                                            style={{ backgroundColor: getColor(val) }}
                                            title={`Score: ${val}`}
                                        >
                                            {/* {val} */}
                                        </td>
                                    ))}
                                </tr>
                            ))}
                        </tbody>
                    </table>
                </div>
            </div>
        </div>
    );
}
