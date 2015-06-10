#include "nlopt.h"
#include "nlopt.hpp"

#include "Models/Base.h"
#include "Algorithm.h"

#include "ofLog.h"

namespace ofxNonLinearFit {
	template<class Model>
	class Fit {
	public:
		Fit(Algorithm algorithm = Algorithm(nlopt::LN_BOBYQA, ofxNonLinearFit::Algorithm::GlobalGradientless),int paramCount =  Model().getParameterCount() ) :
			algorithm(algorithm) {
			this->optimiser = nlopt_create(this->algorithm.getCType(),paramCount);
			this->clearSettings();
		}

		virtual ~Fit() {
			clear();
		}
        
        void clear(){
            nlopt_destroy(this->optimiser);
        }

		///set some parameters in the model if you want to use those as starting parameters
		///since C++ language does not have "where" keyword, we can't access Model::DataSet in the
		///function definition, and must use a void * instead.
		///returns residual or throws string
		bool optimise(Model & model, const void * dataSet, double * residual = NULL) { 
			bool localResidual = (residual == NULL);
			if(localResidual) {
				residual = new double;
			}

			if(!model.isReady()) {
				model.initialiseParameters();
			}
            lowerResidual = 9999999999999;
			Payload payload = {model, dataSet};
			nlopt_func function = & Fit<Model>::minimiserFunction;
//            nlopt_set_ftol_abs(this->optimiser,std::numeric_limits<double>::epsilon());
//            nlopt_set_xtol_abs1(this->optimiser,std::numeric_limits<double>::epsilon());
            nlopt_set_stopval(this->optimiser,stopval ) ;
            nlopt_set_maxeval(this->optimiser, maxEval);
            nlopt_set_maxtime(this->optimiser, maxTime);
            
            vector<double> _upperBound(model.getParameterCount(),upperBound);
            vector<double> _lowerBound(model.getParameterCount(),lowerBound);
//            cout << _upperBound.size() << endl;
            for(int i = 0 ;i<ignoredParams.size() ; i ++ ){
                _lowerBound[ignoredParams[i]] = 0;
                _upperBound[ignoredParams[i]] = 0;
            }
            
//            if(upperBound!=lowerBound){
//                nlopt_set_lower_bounds(this->optimiser,&_lowerBound[0]);
//                nlopt_set_upper_bounds(this->optimiser,&_upperBound[0]);
//            }
//			
            nlopt_set_min_objective(this->optimiser, function, &payload);
			auto result = nlopt_optimize(this->optimiser, model.getParameters(), residual);
            cout << toString(result) << " " << *residual << endl;
			if (result < 0 && result != -4) { // we take something that is roundoff limited as being a success for convenience
				ofLogWarning("ofxNonLinearFit") << "Fit failed :" << toString(result);
			}

			if (localResidual) {
				delete residual;
			}

			return result >= 0;
		}

		void clearSettings() {
			nlopt_set_stopval(this->optimiser, 1e-12);
		}

		void forceStop() {
			nlopt_force_stop(this->optimiser);
		}

		const Algorithm & getAlgorithm() const {
			return this->algorithm;
		}

		static string toString(nlopt_result result) {
			switch(result) {
			case 1:
				return "success";
			case 2:
				return "stop value reached";
			case 3:
				return "ftol reached";
			case 4:
				return "xtol reached";
			case 5:
				return "maxeval reached";
			case 6:
				return "max time reached";
			case -1:
				return "failure";
			case -2:
				return "invalid arguments";
			case -3:
				return "out of memory";
			case -4:
				return "roundoff limited";
			case -5:
				return "forced stop";
			}
			return "code not known";
		}
        
        double maxTime = -1;
        int maxEval = -1;
        double lowerBound = 0;
        double upperBound=0;
        vector<int> ignoredParams;
        double stopval = 10e-6;
        
        
        static double lowerResidual;
        
	protected:
		struct Payload {
			Model & model;
			const void * dataSet;
		};

		static double minimiserFunction(unsigned n, const double * x, double * grad, void * data) {
			//get our useful data out
			const Payload & payload = * (Payload *) data;
			auto & model = payload.model;
			
			//update the model
			model.setParameters(x);
            double residual = model.getResidualOnSet(payload.dataSet);
            if(lowerResidual>residual){
//                cout <<
                (lowerResidual = residual)
//                << endl
                ;}
//            else{
//                cout << "s" << endl;
//            }
			//calc residual
			return residual;
		}

		nlopt_opt optimiser;
		Algorithm algorithm;

	};
    
    template<class Model>
    double Fit<Model>::lowerResidual;
}