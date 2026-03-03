import React from 'react';

export default function StatsCards({ alignment, summary }) {
    return (
        <div className="grid grid-cols-2 md:grid-cols-4 gap-6">
            <div className="bg-bio-bg border border-bio-border rounded-xl p-5 flex flex-col justify-between">
                <h4 className="text-slate-400 text-sm font-semibold uppercase tracking-wider mb-2">Algorithm</h4>
                <div className="text-lg text-bio-cyan font-bold leading-tight">
                    {alignment.algorithm.replace('(', '\n(')}
                </div>
            </div>

            <div className="bg-bio-bg border border-bio-cyan/30 rounded-xl p-5 flex flex-col justify-between glow-cyan relative overflow-hidden">
                <div className="absolute -right-4 -top-4 w-20 h-20 bg-bio-cyan/10 rounded-full blur-xl"></div>
                <h4 className="text-slate-400 text-sm font-semibold uppercase tracking-wider mb-2">Alignment Score</h4>
                <div className="text-4xl text-white font-bold font-mono">
                    {alignment.score}
                </div>
                <div className="text-xs text-slate-500 mt-2">Optimal DP Score</div>
            </div>

            <div className="bg-bio-bg border border-bio-green/30 rounded-xl p-5 flex flex-col justify-between relative overflow-hidden">
                <div className="absolute -right-4 -top-4 w-20 h-20 bg-bio-green/10 rounded-full blur-xl"></div>
                <h4 className="text-slate-400 text-sm font-semibold uppercase tracking-wider mb-2">Identity</h4>
                <div className="flex items-end gap-2">
                    <div className="text-4xl text-bio-green font-bold font-mono">
                        {alignment.identity_percentage.toFixed(1)}<span className="text-2xl">%</span>
                    </div>
                </div>
                <div className="text-xs text-slate-500 mt-2 flex justify-between">
                    <span>Match: <b className="text-bio-green">{alignment.matches}</b></span>
                    <span>Miss: <b className="text-bio-red">{alignment.mismatches}</b></span>
                    <span>Gap: <b className="text-bio-amber">{alignment.gaps}</b></span>
                </div>
            </div>

            <div className={`bg-bio-bg border rounded-xl p-5 flex flex-col justify-between relative overflow-hidden
        ${summary.total_mutations > 0 ? 'border-bio-red/50 shadow-[0_0_15px_rgba(255,71,87,0.15)]' : 'border-bio-green/30'}`}
            >
                <div className={`absolute -right-4 -top-4 w-20 h-20 rounded-full blur-xl ${summary.total_mutations > 0 ? 'bg-bio-red/10' : 'bg-bio-green/10'}`}></div>
                <h4 className="text-slate-400 text-sm font-semibold uppercase tracking-wider mb-2">Mutations</h4>
                <div className="flex items-end gap-2">
                    <div className={`text-4xl font-bold font-mono ${summary.total_mutations > 0 ? 'text-bio-red animate-pulse' : 'text-bio-green'}`}>
                        {summary.total_mutations}
                    </div>
                </div>
                <div className="text-xs text-slate-500 mt-2">
                    {summary.critical_count + summary.high_count} High/Critical Risk
                </div>
            </div>
        </div>
    );
}
