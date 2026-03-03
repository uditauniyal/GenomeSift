import React, { useState, useEffect } from 'react';
import './index.css';

// Layout Components
import Header from './components/Header';
import Sidebar from './components/Sidebar';
import HeroSection from './components/HeroSection';

// Pipeline Components
import Step1DataInput from './components/Step1DataInput';
import Step2Configure from './components/Step2Configure';
import Step3Results from './components/Step3Results';
import Footer from './components/Footer';

function App() {
    const [currentStep, setCurrentStep] = useState(1);
    const [samples, setSamples] = useState({ references: [], patients: [] });

    // Pipeline State
    const [referenceFile, setReferenceFile] = useState(null);
    const [patientFile, setPatientFile] = useState(null);
    const [config, setConfig] = useState({
        algorithm: 'global',
        scoring: { match: 1, mismatch: -1, gap: -2 },
        gene: 'HBB_Healthy_Reference'
    });

    // App Execution State
    const [isLoading, setIsLoading] = useState(false);
    const [loadingMessage, setLoadingMessage] = useState('');
    const [results, setResults] = useState(null);
    const [error, setError] = useState(null);

    useEffect(() => {
        // Fetch available samples on load
        fetch('/api/samples')
            .then(res => res.json())
            .then(data => {
                if (!data.status || data.status !== 'error') {
                    setSamples(data);
                } else {
                    console.error(data.message);
                }
            })
            .catch(err => console.error("Error fetching samples:", err));
    }, []);

    const handleRunAnalysis = async (runConfig = config, runRef = referenceFile, runPat = patientFile) => {
        if (!runRef || !runPat) {
            setError("Please select both a reference and patient sequence.");
            return;
        }

        setError(null);
        setIsLoading(true);
        setLoadingMessage('Initializing C++ Engine...');

        try {
            setTimeout(() => setLoadingMessage('Aligning sequences...'), 800);
            setTimeout(() => setLoadingMessage('Detecting mutations...'), 1600);

            const payload = {
                reference_path: runRef.path,
                patient_path: runPat.path,
                algorithm: runConfig.algorithm,
                gene: runConfig.gene,
                match: runConfig.scoring.match,
                mismatch: runConfig.scoring.mismatch,
                gap: runConfig.scoring.gap
            };

            const response = await fetch('/api/align', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify(payload)
            });

            const data = await response.json();

            if (data.status === 'success') {
                setResults(data);
                setCurrentStep(3);
                // smooth scroll to results after a short delay to allow dom render
                setTimeout(() => {
                    document.getElementById('step3-results')?.scrollIntoView({ behavior: 'smooth' });
                }, 100);
            } else {
                setError(data.message || "An unknown error occurred");
            }
        } catch (err) {
            setError("Failed to connect to backend server. Ensure Flask is running.");
            console.error(err);
        } finally {
            setIsLoading(false);
        }
    };

    const handeQuickDemo = () => {
        const defaultRef = samples.references.find(s => s.name === 'HBB_Healthy_Reference') || samples.references[0];
        const defaultPat = samples.patients.find(s => s.name === 'HBB_Patient_002') || samples.patients[0];

        if (!defaultRef || !defaultPat) {
            setError("Sample sequences missing! Could not run quick demo.");
            return;
        }

        setReferenceFile(defaultRef);
        setPatientFile(defaultPat);

        const demoConfig = {
            algorithm: 'global',
            scoring: { match: 1, mismatch: -1, gap: -2 },
            gene: 'HBB_Healthy_Reference'
        };
        setConfig(demoConfig);

        // Animate briefly through steps
        setCurrentStep(1);
        setTimeout(() => setCurrentStep(2), 400);
        setTimeout(() => {
            handleRunAnalysis(demoConfig, defaultRef, defaultPat);
        }, 800);
    };

    const handleReset = () => {
        setResults(null);
        setError(null);
        setCurrentStep(1);
        window.scrollTo({ top: 0, behavior: 'smooth' });
    };

    return (
        <div className="min-h-screen bg-bio-bg text-slate-200 font-sans flex flex-col">
            <Header />

            <div className="flex flex-1 pt-16">
                <Sidebar currentStep={currentStep} setCurrentStep={setCurrentStep} resultsReady={!!results} />

                <main className="flex-1 w-full max-w-7xl mx-auto px-4 sm:px-8 py-8 ml-16 md:ml-20">
                    <HeroSection onQuickDemo={handeQuickDemo} onStart={() => document.getElementById('step1-input')?.scrollIntoView({ behavior: 'smooth' })} />

                    {error && (
                        <div className="mb-8 p-4 bg-bio-red/10 border border-bio-red/50 rounded-lg flex items-start gap-4 animate-in fade-in slide-in-from-top-4">
                            <span className="text-bio-red text-2xl">⚠️</span>
                            <div>
                                <h3 className="font-bold text-bio-red">Analysis Error</h3>
                                <p className="text-red-200">{error}</p>
                            </div>
                        </div>
                    )}

                    <div className="space-y-16 pb-32">
                        <section id="step1-input">
                            <Step1DataInput
                                samples={samples}
                                referenceFile={referenceFile}
                                setReferenceFile={setReferenceFile}
                                patientFile={patientFile}
                                setPatientFile={setPatientFile}
                                onContinue={() => {
                                    setCurrentStep(2);
                                    document.getElementById('step2-config')?.scrollIntoView({ behavior: 'smooth' });
                                }}
                            />
                        </section>

                        <section id="step2-config" className={currentStep >= 2 ? 'opacity-100 transition-opacity duration-500' : 'opacity-30 pointer-events-none transition-opacity duration-500'}>
                            <Step2Configure
                                config={config}
                                setConfig={setConfig}
                                onRun={handleRunAnalysis}
                                isLoading={isLoading}
                                loadingMessage={loadingMessage}
                                canRun={!!referenceFile && !!patientFile}
                            />
                        </section>

                        {results && (
                            <section id="step3-results" className="pt-8 border-t border-bio-border/50">
                                <Step3Results results={results} onReset={handleReset} />
                            </section>
                        )}
                    </div>
                </main>
            </div>

            <Footer />
        </div>
    );
}

export default App;
