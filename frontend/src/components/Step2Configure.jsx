import React from 'react';

export default function Step2Configure({ config, setConfig, onRun, isLoading, loadingMessage, canRun }) {
    const updateConfig = (key, val) => {
        setConfig(prev => ({ ...prev, [key]: val }));
    };

    const updateScoring = (key, val) => {
        setConfig(prev => ({
            ...prev,
            scoring: { ...prev.scoring, [key]: val }
        }));
    };

    return (
        <div className="bg-bio-card/80 border border-bio-border rounded-xl p-8 shadow-xl relative overflow-hidden">
            {/* Loading Overlay */}
            {isLoading && (
                <div className="absolute inset-0 z-50 bg-bio-bg/80 backdrop-blur-sm flex flex-col items-center justify-center">
                    <div className="w-16 h-16 text-bio-cyan animate-[spin_3s_linear_infinite] mb-6">
                        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
                            <path d="M4 4v16M20 4v16M4 8h16M4 16h16M8 4c0 3-4 6-4 8s4 5 4 8M16 4c0 3 4 6 4 8s-4 5-4 8" className="opacity-90" />
                        </svg>
                    </div>
                    <h3 className="text-2xl font-bold text-white mb-2">{loadingMessage}</h3>
                    <p className="text-bio-cyan animate-pulse">Running C++ Engine...</p>
                </div>
            )}

            <div className="flex items-center gap-4 mb-8">
                <div className="flex items-center justify-center w-10 h-10 rounded-full bg-bio-cyan/20 border border-bio-cyan text-bio-cyan font-bold text-xl glow-cyan">
                    2
                </div>
                <h2 className="text-3xl font-bold">Configure Analysis</h2>
            </div>

            <div className="grid grid-cols-1 md:grid-cols-3 gap-6">

                {/* Algorithm Card */}
                <div className="bg-bio-bg border border-bio-border p-5 rounded-lg">
                    <h3 className="text-lg font-bold text-white mb-4">Algorithm</h3>
                    <div className="space-y-3">
                        <label className={`block cursor-pointer p-4 rounded-lg border transition-all ${config.algorithm === 'global' ? 'bg-bio-cyan/10 border-bio-cyan shadow-[0_0_10px_rgba(0,212,255,0.2)]' : 'border-bio-border hover:border-slate-600'}`}>
                            <div className="flex items-center gap-3">
                                <input type="radio" className="hidden" checked={config.algorithm === 'global'} onChange={() => updateConfig('algorithm', 'global')} />
                                <div className="w-10 h-4 bg-bio-green/20 rounded border border-bio-green"></div>
                                <div>
                                    <div className="font-bold text-white">Global (Needleman-Wunsch)</div>
                                    <div className="text-xs text-slate-400 mt-1">Aligns full sequences end-to-end.</div>
                                </div>
                            </div>
                        </label>

                        <label className={`block cursor-pointer p-4 rounded-lg border transition-all ${config.algorithm === 'local' ? 'bg-bio-cyan/10 border-bio-cyan shadow-[0_0_10px_rgba(0,212,255,0.2)]' : 'border-bio-border hover:border-slate-600'}`}>
                            <div className="flex items-center gap-3">
                                <input type="radio" className="hidden" checked={config.algorithm === 'local'} onChange={() => updateConfig('algorithm', 'local')} />
                                <div className="w-10 h-4 bg-transparent border-t border-b border-dashed border-slate-600 rounded flex items-center justify-center">
                                    <div className="w-4 h-full bg-bio-green"></div>
                                </div>
                                <div>
                                    <div className="font-bold text-white">Local (Smith-Waterman)</div>
                                    <div className="text-xs text-slate-400 mt-1">Finds optimal matching regions.</div>
                                </div>
                            </div>
                        </label>
                    </div>
                </div>

                {/* Scoring Matrix */}
                <div className="bg-bio-bg border border-bio-border p-5 rounded-lg">
                    <h3 className="text-lg font-bold text-white mb-4">Scoring Matrix</h3>
                    <div className="space-y-4">
                        <div className="flex bg-bio-card rounded-md p-1 border border-bio-border">
                            <button className="flex-1 py-1 text-sm bg-bio-cyan text-bio-bg font-bold rounded shadow">Simple</button>
                            <button className="flex-1 py-1 text-sm text-slate-400 hover:text-white transition cursor-not-allowed" disabled>Weighted (Pro)</button>
                        </div>

                        <div className="space-y-3 pt-2">
                            <div className="flex justify-between items-center group">
                                <label className="text-sm font-medium text-slate-300">Match Reward</label>
                                <input type="number" value={config.scoring.match} onChange={e => updateScoring('match', parseInt(e.target.value))} className="w-20 bg-bio-card border border-bio-border rounded px-2 py-1 text-bio-green text-right focus:border-bio-cyan outline-none" />
                            </div>
                            <div className="flex justify-between items-center">
                                <label className="text-sm font-medium text-slate-300">Mismatch Penalty</label>
                                <input type="number" value={config.scoring.mismatch} onChange={e => updateScoring('mismatch', parseInt(e.target.value))} className="w-20 bg-bio-card border border-bio-border rounded px-2 py-1 text-bio-red text-right focus:border-bio-cyan outline-none" />
                            </div>
                            <div className="flex justify-between items-center">
                                <label className="text-sm font-medium text-slate-300">Gap Penalty</label>
                                <input type="number" value={config.scoring.gap} onChange={e => updateScoring('gap', parseInt(e.target.value))} className="w-20 bg-bio-card border border-bio-border rounded px-2 py-1 text-bio-amber text-right focus:border-bio-cyan outline-none" />
                            </div>
                        </div>
                    </div>
                </div>

                {/* Gene Context */}
                <div className="bg-bio-bg border border-bio-border p-5 rounded-lg flex flex-col">
                    <h3 className="text-lg font-bold text-white mb-4">Gene Context Database</h3>
                    <label className="block text-sm text-slate-400 mb-2">Target Gene Variant</label>
                    <select
                        className="w-full bg-bio-card border border-bio-border text-white rounded-md p-3 mb-4 focus:outline-none focus:border-bio-cyan"
                        value={config.gene}
                        onChange={(e) => updateConfig('gene', e.target.value)}
                    >
                        <option value="HBB_Healthy_Reference">HBB (Hemoglobin Beta)</option>
                        <option value="HBB">HBB (Legacy ID)</option>
                        <option value="BRCA1_Reference">BRCA1</option>
                        <option value="CFTR">CFTR</option>
                        <option value="HFE">HFE</option>
                        <option value="MTHFR">MTHFR</option>
                        <option value="FV">FV (Factor V Leiden)</option>
                        <option value="APOE">APOE (Alzheimer Risk)</option>
                        <option value="G6PD">G6PD (Favism)</option>
                        <option value="Unknown">Other / Discover</option>
                    </select>

                    <div className="mt-auto p-3 bg-bio-cyan/5 border border-bio-cyan/20 rounded text-sm text-slate-300">
                        <span className="text-bio-cyan font-bold block mb-1">Context Analysis</span>
                        The mutation detector will map detected variations against known pathogenic SNVs for this specific gene in the disease mapping DB.
                    </div>
                </div>

            </div>

            <div className="mt-10 flex justify-center">
                <button
                    onClick={() => onRun()}
                    disabled={!canRun || isLoading}
                    className={`px-12 py-4 rounded-xl font-bold text-xl uppercase tracking-wider transition-all duration-300
            ${!canRun
                            ? 'bg-slate-700 text-slate-500 cursor-not-allowed'
                            : 'bg-bio-cyan text-bio-bg hover:bg-white hover:scale-105 shadow-[0_0_20px_rgba(0,212,255,0.6)]'
                        }`}
                >
                    {isLoading ? 'Processing...' : 'Run Analysis'}
                </button>
            </div>
        </div>
    );
}
