import React from 'react';

export default function Sidebar({ currentStep, setCurrentStep, resultsReady }) {
    const steps = [
        { id: 1, name: "Data Input", icon: "🧬" },
        { id: 2, name: "Configure", icon: "⚙️" },
        { id: 3, name: "Results", icon: "📊", disabled: !resultsReady }
    ];

    return (
        <aside className="fixed left-0 top-16 bottom-0 w-16 md:w-20 bg-bio-card/50 border-r border-bio-border flex flex-col items-center py-8 z-40">
            <div className="relative flex flex-col justify-between items-center h-64">
                {/* Vertical connector line */}
                <div className="absolute top-4 bottom-4 left-1/2 -ml-[1px] w-[2px] bg-slate-800 z-0"></div>
                <div
                    className="absolute top-4 left-1/2 -ml-[1px] w-[2px] bg-bio-cyan z-0 transition-all duration-700 ease-out shadow-[0_0_8px_rgba(0,212,255,0.8)]"
                    style={{ height: currentStep === 1 ? '0%' : currentStep === 2 ? '50%' : '100%' }}
                ></div>

                {steps.map((step) => {
                    const isActive = currentStep === step.id;
                    const isPast = currentStep > step.id;

                    return (
                        <button
                            key={step.id}
                            onClick={() => {
                                if (!step.disabled) {
                                    setCurrentStep(step.id);
                                    const el = document.getElementById(`step${step.id}-${step.name.toLowerCase().replace(' ', '')}`);
                                    if (el) el.scrollIntoView({ behavior: 'smooth' });
                                }
                            }}
                            disabled={step.disabled}
                            className={`relative z-10 w-12 h-12 rounded-full flex items-center justify-center text-xl transition-all duration-300 
                ${isActive ? 'bg-bio-bg border-2 border-bio-cyan glow-cyan scale-110' :
                                    isPast ? 'bg-bio-cyan/20 border-2 border-bio-cyan/50 text-white' :
                                        'bg-bio-card border-2 border-slate-700 text-slate-500 hover:border-slate-500'}
                ${step.disabled ? 'opacity-50 cursor-not-allowed' : 'cursor-pointer'}
              `}
                            title={step.name}
                        >
                            {step.icon}
                        </button>
                    );
                })}
            </div>
        </aside>
    );
}
