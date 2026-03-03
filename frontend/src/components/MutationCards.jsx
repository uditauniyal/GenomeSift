import React from 'react';

export default function MutationCards({ mutations, summary }) {
    if (mutations.length === 0) {
        return (
            <div className="bg-bio-green/5 border border-bio-green/30 rounded-xl p-8 shadow-lg flex items-center gap-6 justify-center">
                <div className="w-16 h-16 rounded-full bg-bio-green/20 flex items-center justify-center text-4xl text-bio-green">
                    ✓
                </div>
                <div>
                    <h3 className="text-2xl font-bold text-bio-green mb-1">No Mutations Detected</h3>
                    <p className="text-bio-green/70">The patient sequence matches the reference exactly.</p>
                </div>
            </div>
        );
    }

    const getSeverityGlow = (sev) => {
        switch (sev) {
            case 'CRITICAL': return 'border-bio-red shadow-[0_0_15px_rgba(255,71,87,0.4)]';
            case 'HIGH': return 'border-bio-red shadow-[0_0_10px_rgba(255,71,87,0.2)]';
            case 'MEDIUM': return 'border-bio-amber shadow-[0_0_10px_rgba(255,179,71,0.2)]';
            default: return 'border-bio-green';
        }
    };

    const getSeverityBg = (sev) => {
        switch (sev) {
            case 'CRITICAL': return 'bg-bio-red text-white';
            case 'HIGH': return 'bg-bio-red/80 text-white';
            case 'MEDIUM': return 'bg-bio-amber/80 text-black';
            default: return 'bg-bio-green/80 text-black';
        }
    };

    return (
        <div className="bg-bio-card/80 border border-bio-border rounded-xl p-8 shadow-xl">
            <h3 className="text-2xl font-bold text-white mb-6 flex items-center gap-3">
                Detected Mutations
                <span className="bg-bio-red text-white text-xs px-2 py-1 rounded-full">{mutations.length} Found</span>
            </h3>

            <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
                {mutations.map((m, idx) => (
                    <div key={idx} className={`bg-bio-bg rounded-xl border ${getSeverityGlow(m.severity)} p-5 flex flex-col relative overflow-hidden group hover:-translate-y-1 transition-transform`}>

                        {/* Top row */}
                        <div className="flex justify-between items-start mb-4">
                            <span className={`text-[10px] font-bold uppercase tracking-widest px-2 py-1 rounded ${getSeverityBg(m.severity)}`}>
                                {m.severity} RISK
                            </span>
                            <span className="text-xs text-slate-500 bg-black/40 px-2 py-1 rounded">{m.type}</span>
                        </div>

                        {/* Title */}
                        <h4 className="text-slate-300 text-sm font-medium mb-1">Position {m.position}</h4>

                        {/* The Mutation */}
                        <div className="flex items-center gap-4 my-2">
                            <div className="w-12 h-12 rounded bg-black flex items-center justify-center border border-bio-green/30 text-bio-green font-mono text-xl font-bold">
                                {m.reference}
                            </div>
                            <div className="text-slate-500 animate-pulse text-xl">→</div>
                            <div className="w-12 h-12 rounded bg-bio-red/10 flex items-center justify-center border border-bio-red text-bio-red font-mono text-xl font-bold">
                                {m.variant}
                            </div>
                        </div>

                        {/* Disease Info */}
                        <div className="mt-auto pt-4 border-t border-bio-border/50">
                            <div className="flex items-center gap-2 text-bio-cyan mb-1 font-semibold">
                                <span className="text-lg">✚</span>
                                {m.disease !== 'Unknown' ? m.disease : 'Unknown Clinical Significance'}
                            </div>
                            <div className="text-xs text-slate-500">
                                Gene Associated: <span className="text-slate-300">{m.gene}</span>
                            </div>
                        </div>

                        {/* Subtle background icon */}
                        <div className="absolute -bottom-4 -right-4 text-9xl opacity-[0.03] text-white select-none pointer-events-none font-bold">
                            !
                        </div>
                    </div>
                ))}
            </div>

            {/* Severity Chart */}
            <div className="mt-8 pt-6 border-t border-bio-border flex flex-col sm:flex-row gap-4">
                <div className="text-sm text-slate-400 w-32 shrink-0 pt-1">Severity Profile:</div>
                <div className="flex-1 flex h-8 rounded overflow-hidden">
                    {summary.critical_count > 0 && <div style={{ width: `${(summary.critical_count / mutations.length) * 100}%` }} className="bg-bio-red/90 h-full border-r border-black" title="Critical"></div>}
                    {summary.high_count > 0 && <div style={{ width: `${(summary.high_count / mutations.length) * 100}%` }} className="bg-bio-red/60 h-full border-r border-black" title="High"></div>}
                    {summary.medium_count > 0 && <div style={{ width: `${(summary.medium_count / mutations.length) * 100}%` }} className="bg-bio-amber h-full border-r border-black" title="Medium"></div>}
                    {summary.low_count > 0 && <div style={{ width: `${(summary.low_count / mutations.length) * 100}%` }} className="bg-bio-green h-full border-r border-black" title="Low"></div>}
                </div>
            </div>
        </div>
    );
}
